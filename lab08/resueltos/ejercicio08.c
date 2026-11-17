#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */

/* prototipo para la función manejada por el thread */
void handler ( void *ptr );

/* variables globales*/
/* semaforos son declarados como globales y pueden ser accesados 
   en main() y en la función thread,
   aquí los semáforos son usados como un mutex */
sem_t mutex;
int counter; /* shared variable */

int main()
{
    int i[2];
    pthread_t thread_a;
    pthread_t thread_b;
    
    i[0] = 0; /* argumento para threads */
    i[1] = 1;
    
    sem_init(&mutex, 0, 1);      /* inicializar mutex a 1 – semaforo binario */
                                 /* segundo parametro = 0 – semaforo es local */
                                 
    /* Nota tu puedes verificar si el thread ha sido exitosamente creado retornando el valor de 
       pthread_create */                                 
    pthread_create (&thread_a, NULL, (void *) &handler, (void *) &i[0]);
    pthread_create (&thread_b, NULL, (void *) &handler, (void *) &i[1]);
    
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    sem_destroy(&mutex); /* destruye el semaforo */
                  
    /* exit */  
    exit(0);
} /* main() */

void handler ( void *ptr )
{
    int x; 
    x = *((int *) ptr);
    printf("Thread %d: esperando para entrar a la sección critica...\n", x);
    sem_wait(&mutex);       /* down semaphore */
    /* START CRITICAL REGION */
    printf("Thread %d: Ahora en la sección critica...\n", x);
    printf("Thread %d: Valor del contador: %d\n", x, counter);
    printf("Thread %d: Incrementa el contador...\n", x);
    counter++;
    printf("Thread %d: New Valor del contador: %d\n", x, counter);
    printf("Thread %d: Sale de la región critica...\n", x);
    /* END CRITICAL REGION */    
    sem_post(&mutex);       /* up semaphore */
    
    pthread_exit(0); /* exit thread */
}
