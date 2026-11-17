#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

// Estructura de datos Buffer
#define MAX_BUFFERS 10
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;

sem_t mutex_sem, buffer_count_sem, spool_signal_sem;

void *producer (void *arg);
void *spooler (void *arg);

int main (int argc, char **argv)
{
    pthread_t tid_producer [10], tid_spooler;
    int i, r;

    // Inicialización
    buffer_index = buffer_print_index = 0;

    //  semaforo de exclusión mutua, mutex_sem con un valor inicial 1.
    if (sem_init (&mutex_sem, 0, 1) == -1) {
        perror ("sem_init"); exit (1);
    }
    
    // contador del semaforo, indicando del número de buffers disponibles. Valor incial = MAX_BUFFERS
    if (sem_init (&buffer_count_sem, 0, MAX_BUFFERS) == -1) {
        perror ("sem_init"); exit (1);
    }

    // Contador del semanforo, indicando el número de cadenas para ser impresas. Valor inicial = 0
    if (sem_init (&spool_signal_sem, 0, 0) == -1) {
        perror ("sem_init"); exit (1);
    }

    // Creando spooler
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }

    // Creando 10 hilos productores
    int thread_no [10];
    for (i = 0; i < 10; i++) {
        thread_no [i] = i;
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    }
    // Esperando a los productores terminar
    for (i = 0; i < 10; i++)
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
    // No hay mas cadenas para imprimir? Esperar por la señal spool_signal_sem para comenzar en 0
    int semval;
    while (1) {
        if (sem_getvalue (&spool_signal_sem, &semval)== -1)
            perror ("sem_getvalue");
        if (!semval) break;
        sleep (1);
    }
    // terminar spooler
    if ((r = pthread_cancel (tid_spooler)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }
    
    // Destruir semáforos

    if (sem_destroy (&mutex_sem) == -1) {
        perror ("sem_destroy"); exit (1);
    }

    if (sem_destroy (&mutex_sem) == -1) {
        perror ("sem_destroy"); exit (1);
    }

    if (sem_destroy (&mutex_sem) == -1) {
        perror ("sem_destroy"); exit (1);
    }
    exit (0);
}

// productor: produce cadenas para imprimir
// Debe haber multiples hilos productores
void *producer (void *arg)
{
    // Crear 10 cadenas y terminar
    int i;
    int my_id = *((int *) arg);
    int count = 0;

    for (i = 0; i < 10; i++) {
        // get a buffer: P (buffer_count_sem);
        if (sem_wait (&buffer_count_sem) == -1) {
	    perror ("sem_wait: buffer_count_sem"); exit (1);
        }
    
        /* Hay multiples productores, debemos asegurarnos que solamente un producto usa el 
buffer_index al mismo tiempo.  */
        // P (mutex_sem);
        if (sem_wait (&mutex_sem) == -1) {
	    perror ("sem_wait: mutex_sem"); exit (1);
        }

	    // Sección critica
            int j = buffer_index;
            buffer_index++;
            if (buffer_index == MAX_BUFFERS)
                buffer_index = 0;

        // Libera mutex sem: V (mutex_sem)
        if (sem_post (&mutex_sem) == -1) {
	    perror ("sem_post: mutex_sem"); exit (1);
        }
    
	// Produce una cadena
        sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);
	// Dice al spooler que hay una cadena para ser impresa: V (spool_signal_sem);
        if (sem_post (&spool_signal_sem) == -1) {
	    perror ("sem_post: spool_signal_sem"); exit (1);
        }
    
        // Take a nap
        sleep (1);
    }
}

// Hay solamente un thread spooler 
void *spooler (void *arg)
{

    while (1) {  // forever
        // hay un thread para imprirmir? P (spool_signal_sem);
        if (sem_wait (&spool_signal_sem) == -1) {
	    perror ("sem_wait: spool_signal_sem"); exit (1);
        }
    
        printf ("%s", buf [buffer_print_index]);

        /* Desde que haya solamente un thread (spooler) usando el 
           buffer_print_index, mutex semaphore no es necesario*/
        buffer_print_index++;
        if (buffer_print_index == MAX_BUFFERS)
           buffer_print_index = 0;

        /* Contenido de un buffer a ser impreso.
           Uno o mas buffers están disponibles para ser productores.
           Libera buffer: V (buffer_count_sem);  */
        if (sem_post (&buffer_count_sem) == -1) {
	    perror ("sem_post: buffer_count_sem"); exit (1);
        }
    }
}
