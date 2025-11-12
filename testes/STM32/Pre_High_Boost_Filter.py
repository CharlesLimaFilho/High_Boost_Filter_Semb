import serial
import time
import numpy as np
import os 

# --- Configuração global ---
SLICE_HEIGHT = 30  # Altura de cada fatia (conforme STM32)
SLICE_WIDTH = 90   # Largura de cada fatia (conforme STM32)
n_div = 3          # Número de divisões verticais
OUTPUT_FILE = 'outputs/resultado_high_boost.pgm' 

# --- Configuração da porta serial ---
porta = 'COM6' 
baudrate = 115200
TIMEOUT_MS = 10000  # Timeout em ms para recebimento 

def read_pgm_p2(path):
    """Lê um arquivo PGM P2 e retorna a matriz de pixels e o valor máximo."""
    try:
        with open(path, 'r') as f:
            magic = f.readline().strip()
            if magic != 'P2':
                raise ValueError('Não é P2 PGM')
            
            line = f.readline().strip()
            while line.startswith('#'):
                line = f.readline().strip()
            
            parts = line.split()
            width, height = int(parts[0]), int(parts[1])
            max_val = int(f.readline().strip())
            
            data = []
            for line in f:
                if line.startswith('#'):
                    continue
                data.extend([int(tok) for tok in line.split()])
                
            arr = np.array(data, dtype=np.uint8) 
            arr = arr.reshape((height, width))
            return arr, max_val
    except FileNotFoundError:
        print(f"Erro: Arquivo não encontrado em {path}")
        exit()
    except Exception as e:
        print(f"Erro ao ler PGM: {e}")
        exit()

def separador(arr):
    """
    Divide a matriz em fatias de 30x90 pixels com bordas de sobreposição.
    - Primeira linha: borda superior (zeros)
    - Próximas 30 linhas: core da fatia
    - Última linha: borda inferior (próxima linha ou zeros)
    """
    fatias = []
    height, width = arr.shape
    core_height = SLICE_HEIGHT  # 30 pixels
    
    for i in range(n_div):
        posi_inicial = i * core_height
        posi_final = min(posi_inicial + core_height, height)
        
        # Borda superior (linha anterior ou zero padding)
        if i == 0:
            borda_superior = np.zeros((1, width), dtype=np.uint8)
        else:
            borda_superior = arr[posi_inicial - 1].reshape(1, width)
            
        # Core da fatia
        fatia_principal = arr[posi_inicial : posi_final]
        
        # Borda inferior (próxima linha ou zero padding)
        if posi_final < height:
            borda_inferior = arr[posi_final].reshape(1, width)
        else:
            borda_inferior = np.zeros((1, width), dtype=np.uint8)
            
        # Monta a fatia completa: 1 + 30 + 1 = 32 linhas
        fatia_completa = np.vstack((borda_superior, fatia_principal, borda_inferior))
        fatias.append(fatia_completa)
    
    return fatias


def send_fatia(uart, fatia, slice_index):
    """
    Envia uma fatia para a STM32 com protocolo:
    - Header: [0xAA, altura, largura]
    - Dados: bytes brutos da imagem
    """
    height, width = fatia.shape
    
    if height > 255 or width > 255:
        print(f"❌ ERRO: Dimensões da fatia ({height}x{width}) excedem 255 bytes")
        return False

    # Converte a fatia para bytes
    data = fatia.astype(np.uint8).tobytes()
    
    # Monta o header
    header = bytes([0xAA, height, width])
    
    print(f"\n📤 Enviando Fatia {slice_index + 1}/{n_div}")
    print(f"   Dimensões: {height}x{width} | Total: {len(header) + len(data)} bytes")
    
    try:
        uart.write(header)
        time.sleep(0.1)  # Pequena pausa entre header e dados
        uart.write(data)
        time.sleep(0.5)  # Pausa para processamento
        print(f"✅ Fatia {slice_index + 1} enviada com sucesso")
        return True
    except Exception as e:
        print(f"❌ Erro ao enviar fatia: {e}")
        return False

def receive_slice(uart, slice_index, timeout_ms=TIMEOUT_MS):
    """
    Recebe a fatia processada da STM32 via serial.
    Espera por: "--- Processamento Completo ---"
    Coleta apenas as linhas do core (sem bordas de padding)
    """
    print(f"\n📥 Aguardando resposta da STM32 para fatia {slice_index + 1}...")
    
    start_time = time.time()
    timeout_sec = timeout_ms / 1000.0
    
    raw_lines = []
    collecting = False
    
    while (time.time() - start_time) < timeout_sec:
        if uart.in_waiting > 0:
            try:
                line = uart.readline().decode('utf-8', errors='ignore').strip()
                
                if line:
                    print(f"   {line}")
                
                # Inicia coleta de dados
                if "--- Matriz High Boost Processada ---" in line:
                    collecting = True
                    print("   📊 Iniciando coleta de dados...")
                    continue
                
                # Finaliza coleta
                if "--- Processamento Completo ---" in line:
                    collecting = False
                    print("   ✅ Coleta finalizada!")
                    break
                
                # Coleta as linhas de dados
                if collecting and line:
                    # Descarta linhas de cabeçalho
                    if not any(x in line for x in ['Fator', 'Dimensões', '---', 'Matriz']):
                        raw_lines.append(line)
                        
            except Exception as e:
                print(f"   ⚠️  Erro ao decodificar: {e}")
                continue
        else:
            time.sleep(0.01)  # Pequena pausa se não houver dados
    
    if not raw_lines:
        print(f"   ❌ Nenhuma linha de dados recebida!")
        return None
    
    # Converte para matriz NumPy
    try:
        data = []
        for line in raw_lines:
            pixels = [int(p) for p in line.split() if p]
            if pixels:
                data.append(pixels)
        
        if not data:
            print(f"   ❌ Falha ao parsear dados!")
            return None
        
        matrix = np.array(data, dtype=np.uint8)
        print(f"   ✅ Matriz recebida: {matrix.shape}")
        return matrix
        
    except Exception as e:
        print(f"   ❌ Erro ao processar dados: {e}")
        return None


def write_pgm_p2(path, matrix, max_val=255):
    """Escreve a matriz NumPy em um arquivo PGM P2."""
    height, width = matrix.shape
    with open(path, 'w') as f:
        f.write('P2\n')
        f.write(f'# Imagem Processada com High Boost Filter - {height}x{width}\n')
        f.write(f'{width} {height}\n')
        f.write(f'{max_val}\n')
        # Escreve os pixels
        for row in matrix:
            f.write(' '.join(map(str, row)) + '\n')
    print(f"\n✅ Resultado final salvo como PGM P2 em: {path}")

if __name__ == '__main__':
    print("=" * 60)
    print("🚀 SYSTEM: High Boost Filter via STM32")
    print("=" * 60)
    
    # --- ETAPA 1: Leitura da imagem ---
    file_path = 'D:/Programação/Python/High_Boost_Filter/inputs/mona_lisa.ascii (1).pgm'
    print(f"\n📖 Lendo imagem: {file_path}")
    
    arr, max_val = read_pgm_p2(file_path) 
    print(f"   ✅ Imagem carregada: {arr.shape[0]} linhas × {arr.shape[1]} colunas")
    print(f"   Valor máximo: {max_val}")
    
    # --- ETAPA 2: Divisão em fatias ---
    print(f"\n✂️  Dividindo imagem em {n_div} fatias...")
    fatias = separador(arr)
    
    for i, fatia in enumerate(fatias):
        print(f"   Fatia {i+1}: {fatia.shape[0]}×{fatia.shape[1]} pixels")
    
    # --- ETAPA 3: Conexão serial ---
    print(f"\n🔌 Conectando à porta {porta} @ {baudrate} bps...")
    
    try:
        uart = serial.Serial(porta, baudrate, timeout=1)
        time.sleep(2) 
        uart.reset_input_buffer()
        print("   ✅ Conexão estabelecida!")
        
        all_slices = []
        
        # --- ETAPA 4: Loop de processamento ---
        for cont in range(n_div):
            # Envia fatia
            if not send_fatia(uart, fatias[cont], cont):
                print(f"❌ Falha ao enviar fatia {cont + 1}")
                break
            
            # Recebe resposta
            processed_slice = receive_slice(uart, cont)
            
            if processed_slice is not None:
                all_slices.append(processed_slice)
            else:
                print(f"❌ Falha ao receber fatia {cont + 1}")
                break
        
        # --- ETAPA 5: Reconstrução e salvamento ---
        if all_slices and len(all_slices) == n_div:
            print(f"\n🔧 Reconstruindo imagem final...")
            final_matrix = np.vstack(all_slices)
            print(f"   Imagem final: {final_matrix.shape[0]}×{final_matrix.shape[1]} pixels")
            
            write_pgm_p2(OUTPUT_FILE, final_matrix, max_val)
            print("=" * 60)
            print("✅ SUCESSO: Processamento concluído!")
            print("=" * 60)
        else:
            print(f"\n❌ ERRO: Apenas {len(all_slices)}/{n_div} fatias foram recebidas")

    except serial.SerialException as e:
        print(f"\n❌ ERRO SERIAL: Não foi possível abrir {porta}")
        print(f"   Verifique se a porta está correta e não está em uso")
        print(f"   Detalhes: {e}")
    except Exception as e:
        print(f"\n❌ ERRO: {e}")
        import traceback
        traceback.print_exc()
    finally:
        if 'uart' in locals() and uart.is_open:
            uart.close()
            print("\n🔌 Porta serial fechada")