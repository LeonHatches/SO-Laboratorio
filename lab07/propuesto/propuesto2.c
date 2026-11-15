#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
int buffer = 0;
int lleno = 0;
pthread_mutex_t mutex;
// Produce datos si esta vacio
void* productor(void* arg) {
    int i = 1;
    while (i <= 5) {
        pthread_mutex_lock(&mutex);
        if (lleno == 0) {
            buffer = i * 10;
            lleno = 1;
            printf("Productor escribio: %d\n", buffer);
            i++;
        }
        pthread_mutex_unlock(&mutex);
        usleep(10000);
    }
    return NULL;
}
// Consume datos si esta lleno
void* consumidor(void* arg) {
    int leidos = 0;
    while (leidos < 5) {
        pthread_mutex_lock(&mutex);
        if (lleno == 1) {
            printf("Consumidor leyo: %d\n", buffer);
            lleno = 0;
            leidos++;
        }
        pthread_mutex_unlock(&mutex);
        usleep(10000);
    }
    return NULL;
}
// Inicia productor y consumidor
int main() {
    pthread_t h_prod, h_cons;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&h_prod, NULL, productor, NULL);
    pthread_create(&h_cons, NULL, consumidor, NULL);
    pthread_join(h_prod, NULL);
    pthread_join(h_cons, NULL);
    return 0;
}
