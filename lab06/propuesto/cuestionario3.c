#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

int x = 0;

void *operar(void *arg) {
    int paso = *(const int *)arg;
    for (int i = 1; i <= 100; i++) {
        int anterior = x;
        sched_yield();
        int nuevo = anterior + paso;
        x = nuevo;
        printf("%s %3d: %d -> %d\n",
               paso == 1 ? "Suma" : "Resta", i, anterior, nuevo);
    }
    return NULL;
}

int main(void) {
    int suma = 1, resta = -1;
    for (int p = 1; p <= 5; p++) {
        pthread_t h1, h2;
        x = 0;
        printf("Prueba %d\n", p);
        if (pthread_create(&h1, NULL, operar, &suma) != 0) {
            fputs("No se pudo crear el hilo de suma.\n", stderr);
            return EXIT_FAILURE;
        }
        if (pthread_create(&h2, NULL, operar, &resta) != 0) {
            pthread_join(h1, NULL);
            fputs("No se pudo crear el hilo de resta.\n", stderr);
            return EXIT_FAILURE;
        }
        if (pthread_join(h1, NULL) != 0 || pthread_join(h2, NULL) != 0) {
            fputs("No se pudo esperar a los hilos.\n", stderr);
            return EXIT_FAILURE;
        }
        printf("Resultado de la prueba %d: %d\n", p, x);
    }
    return EXIT_SUCCESS;
}
