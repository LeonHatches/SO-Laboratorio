#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
struct parametros {
    int id;
    float escalar;
    float matriz[3][3];
};
// Inicializa matriz con valores aleatorios
void init(float m[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m[i][j] = (random() % 10) + 1;
        }
    }
}
// Muestra matriz desde un hilo
void* mostrar_matriz(void* arg) {
    struct parametros* p = (struct parametros*) arg;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.2f\t", p->matriz[i][j]);
        }
        printf("\n");
    }
    return NULL;
}
// Multiplica matriz por un escalar
void* matrizporescalar(void* arg) {
    struct parametros* p = (struct parametros*) arg;
    for (int i = 0; i < 3; i++) {
        printf("Hilo %d multiplicando fila %d\n", p->id, i);
        for (int j = 0; j < 3; j++) {
            p->matriz[i][j] = p->matriz[i][j] * p->escalar;
            sleep(1);
        }
    }
    return NULL;
}
// Ejecuta multiplicacion y visualizacion paralela
int main() {
    pthread_t h1, h2;
    struct parametros p1;
    p1.id = 1;
    p1.escalar = 5.0;
    init(p1.matriz);
    pthread_create(&h1, NULL, matrizporescalar, (void*)&p1);
    pthread_join(h1, NULL);
    pthread_create(&h2, NULL, mostrar_matriz, (void*)&p1);
    pthread_join(h2, NULL);
    return 0;
}
