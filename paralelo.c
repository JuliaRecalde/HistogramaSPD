#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NUM_CATEGORIAS 6
#define NUM_ESTADOS 54
omp_lock_t hist_locks[NUM_CATEGORIAS]; 

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

void procesarEstado(const char *line, int indiceEstado) {
    int categorias[NUM_CATEGORIAS] = {0};
    sscanf(line, "%*[^,],%d,%d,%d,%d,%d,%d\n",
           &categorias[0], &categorias[1], &categorias[2], &categorias[3],
           &categorias[4], &categorias[5]);   
    if (strcmp(estados[indiceEstado], "United States") != 0) {
        #pragma omp parallel for
        for (int i = 0; i < NUM_CATEGORIAS; i++) {
            omp_init_lock(&hist_locks[i]);
        }
        #pragma omp parallel for
        for (int i = 0; i < NUM_CATEGORIAS; i++) {
            omp_set_lock(&hist_locks[i]);
            histograma_final[i] += categorias[i];
            omp_unset_lock(&hist_locks[i]);
        }   
        for (int i = 0; i < NUM_CATEGORIAS; i++) {
            omp_destroy_lock(&hist_locks[i]);
        }
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
    for (int indiceEstado = 0; indiceEstado < NUM_ESTADOS; indiceEstado++) {
        fgets(line, sizeof(line), file);
        sscanf(line, "%255[^,],", estados[indiceEstado]);
        procesarEstado(line, indiceEstado);
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
    double inicio, fin, tiempo;
    inicio = omp_get_wtime();
    abrirArchivo();
    procesarArchivo();
    mostrarHistograma();
    fin = omp_get_wtime();
    tiempo = fin - inicio;
    printf("Tiempo de ejecución: %f segundos\n", tiempo);
    return 0;
}
