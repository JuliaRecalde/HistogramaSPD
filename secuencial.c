#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_CATEGORIAS 6
#define NUM_ESTADOS 54

char *nombre_categorias[NUM_CATEGORIAS] = {
    "Children 0-18",
    "Adults  19-25",
    "Adults  26-34",
    "Adults  35-54",
    "Adults  55-64",
    "65+          "
};

int histograma_final[NUM_CATEGORIAS] = {0};
char estados[NUM_ESTADOS][256];

void procesarEstado(const char *line) {
    int categorias[NUM_CATEGORIAS] = {0};
    sscanf(line, "%*[^,],%d,%d,%d,%d,%d,%d\n",
           &categorias[0], &categorias[1], &categorias[2], &categorias[3],
           &categorias[4], &categorias[5]);
    for (int i = 0; i < NUM_CATEGORIAS; i++) {
        histograma_final[i] += categorias[i];
    }
}

void abrirArchivo() {
    char *filename = "raw_data.csv";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo CSV");
        exit(1);
    }
}

void procesarArchivo()
{
    char *filename = "raw_data.csv";
    FILE *file = fopen(filename, "r");
    char line[1024];
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    for (int estadoIndex = 0; estadoIndex < NUM_ESTADOS; estadoIndex++) {
        fgets(line, sizeof(line), file);
        sscanf(line, "%255[^,],", estados[estadoIndex]);
        procesarEstado(line);
    }
    fclose(file);
}

void mostrarHistograma() {
    printf("Histograma final:\n");
    int escala = 10000000;
    for (int i = 0; i < NUM_CATEGORIAS; i++) {
        printf("%s: ", nombre_categorias[i]);
        for (int j = 0; j < histograma_final[i] / escala; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

int main() {
    clock_t inicio, fin;
    double tiempo;
    inicio = clock();
    abrirArchivo();
    procesarArchivo();
    mostrarHistograma();
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", tiempo);
    return 0;
}