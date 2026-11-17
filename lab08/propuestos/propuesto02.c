#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static sem_t sala, equipo;

static void esperar(sem_t *sem) {
    while (sem_wait(sem) == -1) {
        if (errno == EINTR) continue;
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

static void revisar_hilo(int error) {
    if (error != 0) {
        errno = error;
        perror("pthread");
        exit(EXIT_FAILURE);
    }
}

static void *trabajar(void *arg) {
    int id = *(int *)arg;

    esperar(&sala);
    printf("Hilo %d entra a la sala\n", id);

    // El equipo se adquiere dentro de la sala.
    esperar(&equipo);
    printf("Hilo %d usa el equipo\n", id);
    sleep(1);
    printf("Hilo %d libera el equipo\n", id);
    sem_post(&equipo);

    printf("Hilo %d sale de la sala\n", id);
    sem_post(&sala);
    return NULL;
}

int main(void) {
    pthread_t hilos[4];
    int ids[] = {1, 2, 3, 4};

    if (sem_init(&sala, 0, 2) == -1 || sem_init(&equipo, 0, 1) == -1) {
        perror("sem_init");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 4; i++)
        revisar_hilo(pthread_create(&hilos[i], NULL, trabajar, &ids[i]));
    for (int i = 0; i < 4; i++)
        revisar_hilo(pthread_join(hilos[i], NULL));

    sem_destroy(&equipo);
    sem_destroy(&sala);
    return EXIT_SUCCESS;
}
