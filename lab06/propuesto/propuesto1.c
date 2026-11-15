#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
float matriz[3][3];
struct parametros {
    int id;
    int fila;
    float escalar;
};
// Inicializa matriz con numeros aleatorios
void init(float m[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m[i][j] = (random() % 10) + 1;
        }
    }
}
// Multiplica una fila por escalar
void* multiplicar_fila(void* arg) {
    struct parametros* p = (struct parametros*) arg;
    printf("Hilo %d multiplicando fila %d\n", p->id, p->fila);
    for (int j = 0; j < 3; j++) {
        matriz[p->fila][j] = matriz[p->fila][j] * p->escalar;
        sleep(1);
    }
    return NULL;
}
// Crea un hilo por fila
int main() {
    pthread_t hilos[3];
    struct parametros params[3];
    float escalar;
    printf("Ingrese escalar N: ");
    if (scanf("%f", &escalar) != 1) {
        fputs("Escalar invalido.\n", stderr);
        return EXIT_FAILURE;
    }
    init(matriz);
    for (int i = 0; i < 3; i++) {
        params[i].id = i + 1;
        params[i].fila = i;
        params[i].escalar = escalar;
        pthread_create(&hilos[i], NULL, multiplicar_fila, &params[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.2f\t", matriz[i][j]);
        }
        printf("\n");
    }
    return 0;
}
