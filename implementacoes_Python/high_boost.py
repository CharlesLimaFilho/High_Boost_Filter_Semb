import numpy as np
from scipy import ndimage

def read_pgm_p2(path):
    with open(path, 'r') as f:
        # ler linhas até pegar “P2”
        magic = f.readline().strip()
        if magic != 'P2':
            raise ValueError('Não é P2 PGM')
        # pular comentários
        line = f.readline().strip()
        while line.startswith('#'):
            line = f.readline().strip()
        parts = line.split()
        width, height = int(parts[0]), int(parts[1])
        max_val = int(f.readline().strip())
        # ler pixels
        data = []
        for line in f:
            if line.startswith('#'):
                continue
            data.extend([int(tok) for tok in line.split()])
        arr = np.array(data, dtype=np.float32)
        arr = arr.reshape((height, width))
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

def high_boost_from_pgm(input_path, output_path, a):
    image, max_val = read_pgm_p2(input_path)

    blurred = ndimage.uniform_filter(image, size = 3, mode='constant')
    blurred2 = np.array(blurred, dtype = int)

    # high-pass
    high_pass = image - blurred2

    # high-boost
    high_boost = (a - 1) * image + high_pass


    result = np.array(high_boost, dtype = int)


    result = np.clip(result, 0, max_val)
    write_pgm_p2(output_path, result, max_val)

if __name__ == '__main__':
    high_boost_from_pgm('C:/Dev/repos/git/CLion/semb2/implementacoes_C/inputs/pepper.ascii.pgm', 'outputs/pepper.ascii.pgm', a=2.0)
