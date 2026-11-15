#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void *hilo(void *arg) {
    (void)arg;
    return NULL;
}

int main(int argc, char **argv) {
    char *fin;
    long n = argc == 2 ? strtol(argv[1], &fin, 10) : 100;
    if (argc > 2 || (argc == 2 && (fin == argv[1] || *fin)) ||
        n < 1 || n > 100000) {
        fputs("Indica una cantidad de 1 a 100000.\n", stderr);
        return EXIT_FAILURE;
    }

    struct timespec inicio, fin_medicion;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (long i = 0; i < n; i++) {
        pthread_t h;
        int error = pthread_create(&h, NULL, hilo, NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            return EXIT_FAILURE;
        }
        error = pthread_join(h, NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            return EXIT_FAILURE;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &fin_medicion);
    double us = (fin_medicion.tv_sec - inicio.tv_sec) * 1e6
              + (fin_medicion.tv_nsec - inicio.tv_nsec) / 1e3;
    printf("%ld hilos: %.2f us en promedio\n", n, us / n);
    return EXIT_SUCCESS;
}
