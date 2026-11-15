#include <stdio.h>
#include <pthread.h>
int x = 0;
// Suma uno cien veces
void* sumar(void* arg) {
    for (int i = 0; i < 100; i++) {
        int aux = x;
        for (int j = 0; j < 5000; j++) {}
        x = aux + 1;
    }
    return NULL;
}
// Resta uno cien veces
void* restar(void* arg) {
    for (int i = 0; i < 100; i++) {
        int aux = x;
        for (int j = 0; j < 5000; j++) {}
        x = aux - 1;
    }
    return NULL;
}
// Ejecuta cinco pruebas seguidas
int main() {
    pthread_t h1, h2;
    for (int p = 1; p <= 5; p++) {
        x = 0;
        pthread_create(&h1, NULL, sumar, NULL);
        pthread_create(&h2, NULL, restar, NULL);
        pthread_join(h1, NULL);
        pthread_join(h2, NULL);
        printf("Prueba %d: %d\n", p, x);
    }
    return 0;
}