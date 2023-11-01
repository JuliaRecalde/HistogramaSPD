#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_CATEGORIES 6
#define NUM_STATES 54

char *category_names[NUM_CATEGORIES] = {
    "Children 0-18",
    "Adults  19-25",
    "Adults  26-34",
    "Adults  35-54",
    "Adults  55-64",
    "65+          "
};

int histograma_final[NUM_CATEGORIES] = {0};
char estados[NUM_STATES][256];

void procesarEstado(const char *line) {
    int categorias[NUM_CATEGORIES] = {0};
    sscanf(line, "%*[^,],%d,%d,%d,%d,%d,%d\n",
           &categorias[0], &categorias[1], &categorias[2], &categorias[3],
           &categorias[4], &categorias[5]);
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        histograma_final[i] += categorias[i];
    }
}

void cargarArchivo() {
    char *filename = "raw_data.csv";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo CSV");
        exit(1);
    }
    char line[1024];
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);
    for (int estadoIndex = 0; estadoIndex < NUM_STATES; estadoIndex++) {
        fgets(line, sizeof(line), file);
        sscanf(line, "%255[^,],", estados[estadoIndex]);
        procesarEstado(line);
    }
    fclose(file);
}

void mostrarHistograma() {
    printf("Histograma final:\n");
    int escala = 10000000;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s: ", category_names[i]);
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
    cargarArchivo();
    mostrarHistograma();
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", tiempo);
    return 0;
}