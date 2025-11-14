from time import sleep
import serial
import time
import numpy as np

fatias = 0
linha_inicial = 0
frase = b'Charles\n'
filtrado = []
ser = serial.Serial('COM5',115200)

def read_pgm_p2(path):
    with open(path, 'r') as f:
        magic = f.readline().strip()
        if magic != 'P2':
            raise ValueError(f"{path} não está em formato P2 PGM")
        # pular comentários
        line = f.readline().strip()
        while line.startswith('#'):
            line = f.readline().strip()
        width, height = map(int, line.split())
        max_val = int(f.readline().strip())
        data = []
        for line in f:
            if line.startswith('#'):
                continue
            data.extend([int(tok) for tok in line.split()])
        arr = np.array(data, dtype=np.int32).reshape((height, width))
        return arr, max_val



def write_pgm_p2(path, arr, max_val=255):
    h, w = arr.shape
    with open(path, 'w') as f:
        f.write('P2\n')
        f.write(f'# High-boost result\n')
        f.write(f'{w} {h}\n')
        f.write(f'{max_val}\n')
        for i in range(h):
            row = arr[i, :]
            row_int = [str(int(val)) for val in row]
            f.write(' '.join(row_int))
            f.write('\n')

def sincronizar():
    contsinc = 0
    sincronizado = False
    while not sincronizado:
        print("Sincronizando...")
        comandosinc = ser.read(1)
        print(f"Comando sincronia: {comandosinc.decode('utf-8')}")
        if comandosinc.decode('utf-8') == 'S':
            contsinc = contsinc + 1
            print(f"Contsinc: {contsinc}")
        if contsinc == 2:
            sincronizado = True
            print("Sincronizado \n\n")
            ser.write(bytes('S', 'ascii'))


def receber_stm():
    cont_re = 0
    limite = 29
    if fatias == 1:
        limite = 30
    elif fatias == 2:
        limite = 31
    while cont_re < limite:
        valores = ser.readline()
        arr = valores.decode('utf-8').split()
        filtrado.append(np.array(arr, dtype=float))

        print(arr)
        cont_re = cont_re + 1


def enviar_stm(linha_inicial):
    cont_en = linha_inicial
    for row in imagem:
        linha_str = ' '.join(str(pixel) for pixel in row) + '\n'
        print(f"Linha : {cont_en + 1}")

        print("Aguardando comando 1")
        comando = ser.read(1)
        print(f" Comando atual: {comando.decode('utf-8')}")
        if comando.decode('utf-8') == 'I':
            sleep(0.4)
            ser.write(bytes(str(len(linha_str)), 'ascii'))
            print("Len da linha enviado")
        sleep(0.5)

        print("Aguardando comando 2")

        comando = ser.read(1)
        print(f" Comando atual: {comando.decode('utf-8')}")

        if comando.decode('utf-8') == 'M':
            sleep(0.4)
            ser.write(bytes(linha_str, 'ascii'))
            print("Linha enviada")
        ser.flush()
        cont_en = cont_en + 1
        if cont_en == 30:
            break

#####################################################################################################################


imagem, max_val = read_pgm_p2("C:/Dev/repos/git/CLion/semb2/implementacoes_Python/inputs/mona_lisa.ascii.pgm")

sincronizar()


while fatias < 3:
    enviar_stm(linha_inicial)
    receber_stm()
    fatias = fatias + 1
    for i in range(30):
        imagem = np.delete(imagem, 0, 0)

print("Finalizando...")





output = np.array(filtrado, dtype=np.float32)
write_pgm_p2("C:/Dev/repos/git/CLion/semb2/implementacoes_Python/outputs/24.pgm", output, 255)
ser.close()