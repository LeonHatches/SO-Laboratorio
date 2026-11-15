#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#define N 6000
#define HILOS 4
int lista[N];
long sumas[HILOS];
// Suma una parte del arreglo
void* sumar_parte(void* arg) {
    int id = *(int*) arg;
    int inicio = id * (N / HILOS);
    int fin = (id == HILOS - 1) ? N : inicio + (N / HILOS);
    sumas[id] = 0;
    for (int i = inicio; i < fin; i++) {
        sumas[id] += lista[i];
    }
    return NULL;
}
// Calcula suma paralela con tiempos
int main() {
    pthread_t hilos[HILOS];
    int ids[HILOS];
    struct timeval t0, t1;
    long total = 0;
    srand(1);
    for (int i = 0; i < N; i++) {
        lista[i] = rand() % 100;
    }
    gettimeofday(&t0, NULL);
    for (int i = 0; i < HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, sumar_parte, &ids[i]);
    }
    for (int i = 0; i < HILOS; i++) {
        pthread_join(hilos[i], NULL);
        total += sumas[i];
    }
    gettimeofday(&t1, NULL);
    long tiempo = (t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_usec - t0.tv_usec);
    printf("Suma total (4 hilos): %ld\n", total);
    printf("Tiempo paralelo: %ld microsegundos\n", tiempo);
    return 0;
}
