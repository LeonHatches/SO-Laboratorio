#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

enum { HILOS = 4, VUELTAS = 10000 };
static long contador;
static sem_t acceso;

static void revisar(int resultado, const char *operacion) {
    if (resultado == 0) return;
    if (resultado > 0) errno = resultado;
    perror(operacion);
    exit(EXIT_FAILURE);
}

static void *incrementar(void *arg) {
    (void)arg;
    for (int i = 0; i < VUELTAS; i++) {
        int resultado;
        do {
            resultado = sem_wait(&acceso);
        } while (resultado == -1 && errno == EINTR);
        revisar(resultado, "sem_wait");

        contador++;
        revisar(sem_post(&acceso), "sem_post");
    }
    return NULL;
}

int main(void) {
    pthread_t hilos[HILOS];

    // La cantidad de vueltas se fija en el programa.
    revisar(sem_init(&acceso, 0, 1), "sem_init");
    for (int i = 0; i < HILOS; i++)
        revisar(pthread_create(&hilos[i], NULL, incrementar, NULL),
                "pthread_create");
    for (int i = 0; i < HILOS; i++)
        revisar(pthread_join(hilos[i], NULL), "pthread_join");

    printf("shared = %ld\n", contador);
    revisar(sem_destroy(&acceso), "sem_destroy");
    return EXIT_SUCCESS;
}
