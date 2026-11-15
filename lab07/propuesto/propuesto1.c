#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 6000
#define HILOS 4
int lista[N];
long suma_total = 0;
pthread_mutex_t mutex;
// Acumula suma parcial usando mutex
void* sumar_parte(void* arg) {
    int id = *(int*) arg;
    int inicio = id * (N / HILOS);
    int fin = (id == HILOS - 1) ? N : inicio + (N / HILOS);
    for (int i = inicio; i < fin; i++) {
        pthread_mutex_lock(&mutex);
        suma_total += lista[i];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
// Inicializa hilos y mutex global
int main() {
    pthread_t hilos[HILOS];
    int ids[HILOS];
    srand(1);
    for (int i = 0; i < N; i++) {
        lista[i] = rand() % 100;
    }
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, sumar_parte, &ids[i]);
    }
    for (int i = 0; i < HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }
    printf("Suma total con mutex: %ld\n", suma_total);
    return 0;
}
