# High_Boost_Filter_Semb
Este é um repositório dobre o algoritmo high boost filter utilizado em SEMB.

---

# Descrição
O High Boost Filter é uma algoritmo de processamento de imagens utilizado para realçar os elementos de alta frequência 
sem eliminar os elementos de baixa frequência.

Este algoritmo segue a técnica de [Unsharp Masking](https://en.wikipedia.org/wiki/Unsharp_masking). Primeiramente, 
é gerado uma versão suavizada da imagem, através de um filtro passa-baixa como o [Median Filter](https://en.wikipedia.org/wiki/Median_filter),
depois é aplicada a fórmula típica: `filtrada = original + (original - suavizada) * fator de amplificação`.

---

## Algumas definições
- Elementos de alta frequência: correspondem às transições abruptas, ex.: unhas, contornos, botões em uma camisa,...
- Elementos de baixa frequência: correspondem a áreas com baixa transição, ex.: uma camisa, uma parede de uma cor, o céu,...

## Validações
As validações são feitas utilizando uma implementação em Python.
