#include <stdio.h>
#include <pthread.h>
#define N 4
int A[N][N] = {{1,2,3,4},{5,6,7,8},{9,1,2,3},{4,5,6,7}};
int B[N][N] = {{2,0,1,3},{1,2,0,1},{0,1,2,1},{1,0,1,2}};
int C[N][N];
pthread_mutex_t mutex;
struct celda {
    int fila;
    int col;
};
// Calcula una celda usando mutex
void* multiplicar_celda(void* arg) {
    struct celda* pos = (struct celda*) arg;
    int suma = 0;
    for (int k = 0; k < N; k++) {
        suma += A[pos->fila][k] * B[k][pos->col];
    }
    pthread_mutex_lock(&mutex);
    C[pos->fila][pos->col] = suma;
    printf("Hilo [%d][%d] calculo: %d\n", pos->fila, pos->col, suma);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
// Lanza dieciseis hilos para multiplicar
int main() {
    pthread_t hilos[N * N];
    struct celda datos[N * N];
    int idx = 0;
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            datos[idx].fila = i;
            datos[idx].col = j;
            pthread_create(&hilos[idx], NULL, multiplicar_celda, &datos[idx]);
            idx++;
        }
    }
    for (int i = 0; i < N * N; i++) {
        pthread_join(hilos[i], NULL);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", C[i][j]);
        }
        printf("\n");
    }
    return 0;
}
