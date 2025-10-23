import numpy as np
import matplotlib.pyplot as plt

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

def compare_and_plot(path1, path2, intervals):
    img1, max1 = read_pgm_p2(path1)
    img2, max2 = read_pgm_p2(path2)
    if img1.shape != img2.shape:
        raise ValueError(f"Tamanhos diferentes: {img1.shape} vs {img2.shape}")
    if max1 != max2:
        print("Aviso: valores máximos diferentes, diferenciação poderá ser enviesada")

    diff = np.abs(img1 - img2)
    equal_mask = (diff == 0)
    count_equal = int(np.sum(equal_mask))

    counts = {}
    for (lo, hi) in intervals:
        mask = (diff >= lo) & (diff < hi)
        counts[(lo, hi)] = int(np.sum(mask))

    print(f"Quantidade de pixels exatamente iguais: {count_equal}")
    for (lo, hi), c in counts.items():
        print(f"Intervalo [{lo}, {hi}) → {c} pixels")

    # Plotagem
    fig, axs = plt.subplots(1, 3, figsize=(15, 5))
    axs[0].set_title("Implementação C")
    im0 = axs[0].imshow(img1, cmap='gray', vmin=0, vmax=max1)
    axs[0].axis('off')

    axs[1].set_title("Implementação Python")
    im1 = axs[1].imshow(img2, cmap='gray', vmin=0, vmax=max1)
    axs[1].axis('off')

    axs[2].set_title("Mapa de Diferenças")
    # utilizar escala de cinza para mostrar quantas diferenças – valores maiores = mais claro
    im2 = axs[2].imshow(diff, cmap='gray', vmin=0, vmax=np.max(diff))
    axs[2].axis('off')

    # colorbar para o mapa de diferenças
    cbar = fig.colorbar(im2, ax=axs[2], orientation='vertical', fraction=0.046, pad=0.04)
    cbar.set_label('Diferença absoluta de pixel')
    plt.tight_layout()
    plt.savefig('Mona_lisa_maior_compare.png')
    plt.show()

if __name__ == '__main__':
    pathA = 'C:/Dev/repos/git/CLion/semb2/implementacoes_C/outputs/mona_lisa.ascii_maior.pgm'
    pathB = 'outputs/mona_lisa.ascii_maior.pgm'
    # definir intervalos de diferença
    intervals = [(1, 5), (5, 10), (10, 20), (20, 255)]

    print(pathB)
    compare_and_plot(pathA, pathB, intervals)
