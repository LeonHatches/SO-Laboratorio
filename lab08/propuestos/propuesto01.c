#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static sem_t cambios_lectores, acceso_dato;
static int lectores_activos, dato;

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

static void *leer(void *arg) {
    int id = *(int *)arg;
    esperar(&cambios_lectores);
    // El primer lector impide que un escritor entre.
    if (++lectores_activos == 1) esperar(&acceso_dato);
    printf("Lector %d entra (%d lectores)\n", id, lectores_activos);
    sem_post(&cambios_lectores);

    printf("Lector %d lee %d\n", id, dato);
    sleep(1);

    esperar(&cambios_lectores);
    printf("Lector %d sale\n", id);
    if (--lectores_activos == 0) sem_post(&acceso_dato);
    sem_post(&cambios_lectores);
    return NULL;
}

static void *escribir(void *arg) {
    int id = *(int *)arg;
    esperar(&acceso_dato);
    dato += 10;
    printf("Escritor %d escribe %d\n", id, dato);
    sem_post(&acceso_dato);
    return NULL;
}

int main(void) {
    pthread_t lectores[3], escritores[2];
    int ids[] = {1, 2, 3};

    if (sem_init(&cambios_lectores, 0, 1) == -1 ||
        sem_init(&acceso_dato, 0, 1) == -1) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 3; i++)
        revisar_hilo(pthread_create(&lectores[i], NULL, leer, &ids[i]));
    for (int i = 0; i < 2; i++)
        revisar_hilo(pthread_create(&escritores[i], NULL, escribir, &ids[i]));
    for (int i = 0; i < 3; i++)
        revisar_hilo(pthread_join(lectores[i], NULL));
    for (int i = 0; i < 2; i++)
        revisar_hilo(pthread_join(escritores[i], NULL));

    printf("Dato final: %d\n", dato);
    sem_destroy(&cambios_lectores);
    sem_destroy(&acceso_dato);
    return EXIT_SUCCESS;
}
