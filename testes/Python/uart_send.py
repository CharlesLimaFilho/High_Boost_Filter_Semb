from array import array
from time import sleep
import serial
import time
import numpy as np

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

imagem, max_val = read_pgm_p2("C:/Dev/repos/git/CLion/semb2/implementacoes_Python/inputs/balloons.ascii.pgm");
cont = 0
frase = b'Charles\n'
ser = serial.Serial('COM4',115200)

arr = [b'10 ',b'20 ',b'30 ',b'40\n']
arr2 = [101, 102, 103, 104]

#arr2b = bytes(arr2)
time.sleep(1.8)
while True:
    if cont == 4:
        sleep(1.5)
        valor = ser.readline()
        valor_str = valor.decode('utf-8')
        print(valor_str)
        cont = 0

        valor_split = valor_str.split(" ")
        I = [0, 1, -1]
        valor_split = np.delete(valor_split, I).tolist()
        valor_int = [int(num) for num in valor_split]
    else:
        arr2b = str(arr2[cont])
        arr2b = arr2b + " "
        ser.write(bytes(str(arr2b), 'ascii'))
        print(cont)
        #sleep(0.4)
        cont = cont + 1
        if cont == 4:
            ser.write(b'\n')


   #print("\nValor recebido : " + str(valor))





ser.close()