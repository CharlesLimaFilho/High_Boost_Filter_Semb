/*
 * 14/10/2025
 *
 * Feito por Charles Lima de Brito Filho e Joao Marcelo Cavalcante Pimenta
 * Instituto Federal de Educacao, Ciencia e Tecnologia do Ceara - IFCE
 *
 * Este codigo e livre para uso.
 *
 * Essa e uma implementacao do filtro High Boost, esse filtro possui como objetivo melhorar os elementos de alta
 * frequencia da imagem sem perder os de baixa frequencia, isto e, melhorar a nitidez da imagem.
 *
 * Esta aplicacao utiliza como entradas um path para um arquivo pgm P2 que se deseja utilizar o filtro, um path
 * para um arquivo onde se deseja armazenar o resultado que tambem sera em pgm P2 (caso nao o arquivo nao seja
 * encontrado, um novo sera criado) e o fator de amplificacao da imagem que devera ser maior do que 1.
 *
 * A entrada devera ser uma imagem em baixa qualidade ou embacada e um fator de amplificacao. A saida sera uma imagem
 * com uma maior nitidez.
 *
 * Essa implementacao esta sendo feita como parte do trabalho da disciplina de Sistemas Embarcados.
 */

#include <stdio.h>

// Tamanho maximo da imagem, neste caso sera de 90 x 90
#define MAX_TAM 90

// Valor maximo que cada pixel da imagem pode assumir
#define MAX_VAL 255

int high_boost(char *input_path, char *output_path, float amp) {
    FILE *input, *output;

    // Dados da imagem
    int largura, altura, max_val;

    // Matriz onde sera armazenada a imagem original,
    //o " + 2 " e necessario para a suavizacao da imagem, pois
    //gera um "contorno" de 0 ao redor da imagem.
    int imagem[MAX_TAM + 2][MAX_TAM + 2];

    // Matriz onde sera armazenada a imagem suavizada
    int suavizado[MAX_TAM][MAX_TAM];

    // Matriz onde sera armazenada a imagem filtrada
    int filtrado[MAX_TAM][MAX_TAM];

    // Vetor para a leitura do arquivo pgm
    char line[100];

    // Abre arquivo de entrada
    input = fopen(input_path, "r");
    if (!input) return 1;

    // Le cabecalho, pula informações desnecessarias e coleta altura, largura e valor maximo
    fgets(line, sizeof(line), input); // P2
    fgets(line, sizeof(line), input); // comentário
    fscanf(input, "%d %d", &largura, &altura);
    fscanf(input, "%d", &max_val);

    // Preenche a matrix imagem com 0 para garantir que nao haja lixo
    // e que a tecnica do zero padding seja aplicada corretamente
    for (int i = 0; i < altura + 2; i++) {
        for (int j = 0; j < largura + 2; j++) {
            imagem[i][j] = 0;
        }
    }


    // Lê dados da imagem e armazena na matriz imagem
    for (int i = 1; i < altura + 1; i++) {
        for (int j = 1; j < largura + 1; j++) {
            fscanf(input, "%d", &imagem[i][j]);
        }
    }
    fclose(input);

    // Aplica filtro de suavizacao
    // A imagem e percorrida pixel a pixel enquanto calcula a media
    // somando o valor dos pixels ao redor (3x3) e dividindo por 9
    for (int i = 1; i < altura + 1; i++) {
        for (int j = 1; j < largura + 1; j++) {
            // Calcula media 3x3
            int sum = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    sum += imagem[i + di][j + dj];
                }
            }
            int avg = sum / 9;

            // Garante que os valores da matriz estejam no intervalo [0, 255]
            if (avg < 0) avg = 0;
            if (avg > MAX_VAL) avg = MAX_VAL;

            suavizado[i - 1][j - 1] = avg;
        }
    }


    // Aplica a formula do high boost [(fator de amplificacao - 1) * imagem + (imagem - suavizado)]
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            // Variavel onde se armazena o resultado da formula
            int resultado;

            // Variavel refernte a (imagem - suavizado)
            int high_pass = imagem[i + 1][j + 1] - suavizado[i][j];

            resultado = (amp - 1.0f) * imagem[i + 1][j + 1] + high_pass;

            // Garante que os valores da matriz estejam no intervalo [0, 255]
            if (resultado < 0) resultado = 0;
            if (resultado > MAX_VAL) resultado = MAX_VAL;

            //Insere o resultado na matriz filtrado
            filtrado[i][j] = resultado;
        }
    }

    // Salva resultado (caso o arquivo nao seja encontrado, ele sera criado com o nome no output_path)
    output = fopen(output_path, "w");
    if (!output) return 1;

    // Escreve o cabecalho
    fprintf(output, "P2\n");
    fprintf(output, "# Filtro High-Boost\n");
    fprintf(output, "%d %d\n", largura, altura);
    fprintf(output, "%d\n", max_val);

    // Escreve os dados da imagem
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fprintf(output, "%d", filtrado[i][j]);
            if (j < largura - 1) fprintf(output, " ");
        }
        fprintf(output, "\n");
    }

    // Fecha o arquivo
    fclose(output);
    return 0;
}