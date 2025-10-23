#include <stdio.h>

#define MAX_TAM 400
#define MAX_VAL 255

int high_boost(char *input_path, char *output_path, float amp) {
    FILE *input, *output;
    int largura, altura, max_val;
    int imagem[MAX_TAM + 2][MAX_TAM + 2];
    int suavizado[MAX_TAM][MAX_TAM];
    int filtrado[MAX_TAM][MAX_TAM];
    char line[100];

    // Abre arquivo de entrada
    input = fopen(input_path, "r");
    if (!input) return 1;

    // Lê cabeçalho
    fgets(line, sizeof(line), input); // P2
    fgets(line, sizeof(line), input); // comentário
    fscanf(input, "%d %d", &largura, &altura);
    fscanf(input, "%d", &max_val);

    for (int i = 0; i < altura + 2; i++) {
        for (int j = 0; j < largura + 2; j++) {
            imagem[i][j] = 0;
        }
    }


    // Lê dados da imagem
    for (int i = 1; i < altura + 1; i++) {
        for (int j = 1; j < largura + 1; j++) {
            fscanf(input, "%d", &imagem[i][j]);
        }
    }
    fclose(input);

    // Aplica filtro de suavizacao
    for (int i = 1; i < altura + 1; i++) {
        for (int j = 1; j < largura + 1; j++) {
            // Calcula média 3x3
            int sum = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    sum += imagem[i + di][j + dj];
                }
            }
            int avg = sum / 9;

            // Clamp para [0, 255]
            if (avg < 0) avg = 0;
            if (avg > MAX_VAL) avg = MAX_VAL;

            suavizado[i - 1][j - 1] = avg;
        }
    }


    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            int resultado;
            int high_pass = imagem[i + 1][j + 1] - suavizado[i][j];

            resultado = (amp - 1.0f) * imagem[i + 1][j + 1] + high_pass;

            // Clamp para [0, 255]
            if (resultado < 0) resultado = 0;
            if (resultado > MAX_VAL) resultado = MAX_VAL;

            filtrado[i][j] = resultado;
        }
    }

    // Salva resultado
    output = fopen(output_path, "w");
    if (!output) return 1;

    fprintf(output, "P2\n");
    fprintf(output, "# Filtro High-Boost\n");
    fprintf(output, "%d %d\n", largura, altura);
    fprintf(output, "%d\n", max_val);

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fprintf(output, "%d", filtrado[i][j]);
            if (j < largura - 1) fprintf(output, " ");
        }
        fprintf(output, "\n");
    }

    fclose(output);
    return 0;
}