#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define N 6000
int lista[N];
// Calcula suma secuencial con tiempos
int main() {
    struct timeval t0, t1;
    long total = 0;
    srand(1);
    for (int i = 0; i < N; i++) {
        lista[i] = rand() % 100;
    }
    gettimeofday(&t0, NULL);
    for (int i = 0; i < N; i++) {
        total += lista[i];
    }
    gettimeofday(&t1, NULL);
    long tiempo = (t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_usec - t0.tv_usec);
    printf("Suma total (secuencial): %ld\n", total);
    printf("Tiempo secuencial: %ld microsegundos\n", tiempo);
    return 0;
}
