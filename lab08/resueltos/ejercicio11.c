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

#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"

// Buffer de estructuras de datos
#define MAX_BUFFERS 10
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;

sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;

void *producer (void *arg);
void *spooler (void *arg);

int main (int argc, char **argv)
{
    pthread_t tid_producer [10], tid_spooler;
    int i, r;

    // Inicializado
    buffer_index = buffer_print_index = 0;

    //  exclusión mutua del semaforo, mutex_sem con valor inicial de 1.
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, O_CREAT, 0660, 1)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }
    
    // contador de semaforo, indicando el número de buffers disponible. Valor inicial = MAX_BUFFERS
    if ((buffer_count_sem = sem_open (SEM_BUFFER_COUNT_NAME, O_CREAT, 0660, MAX_BUFFERS)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }

//contador del semaforo, indicando el numero de cadenas a ser impresa, valor inicial =0
    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }

    // Creando un spooler
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }

    // Creando 10 threads productores
    int thread_no [10];
    for (i = 0; i < 10; i++) {
        thread_no [i] = i;
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    }
    // Esperar a los productores terminar
    for (i = 0; i < 10; i++)
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
//No mas cadenas para imprimir? Esperar por la señal del spool_signal_sem para comenzar en 0
    int semval;
    while (1) {
        if (sem_getvalue (spool_signal_sem, &semval)== -1)
            perror ("sem_getvalue");
        if (!semval) break;
        sleep (1);
    }
    // termina el spooler
    if ((r = pthread_cancel (tid_spooler)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }
    
    // Remueve los semaforos

    if (sem_unlink (SEM_MUTEX_NAME) == -1) {
        perror ("sem_unlink"); exit (1);
    }

    if (sem_unlink (SEM_BUFFER_COUNT_NAME) == -1) {
        perror ("sem_unlink"); exit (1);
    }

    if (sem_unlink (SEM_SPOOL_SIGNAL_NAME) == -1) {
        perror ("sem_unlink"); exit (1);
    }

    exit (0);
}

// producer: produce cadenas para ser impresas
// Debe haber multiples threads productores
void *producer (void *arg)
{
    // Crear 10 cadenas para terminar 
    int i;
    int my_id = *((int *) arg);
    int count = 0;

    for (i = 0; i < 10; i++) {
        // conseguir del  buffer: P (buffer_count_sem);
        if (sem_wait (buffer_count_sem) == -1) {
	    perror ("sem_wait: buffer_count_sem"); exit (1);
        }
    
        /* Debe haber multiples productores. Nosotros nos aseguramos que 
           Solamente un productor use buffer_index a la vez.  */
        // P (mutex_sem);
        if (sem_wait (mutex_sem) == -1) {
	    perror ("sem_wait: mutex_sem"); exit (1);
        }

	    // Sección critica
            int j = buffer_index;
            buffer_index++;
            if (buffer_index == MAX_BUFFERS)
                buffer_index = 0;

        // Libera mutex sem: V (mutex_sem)
        if (sem_post (mutex_sem) == -1) {
	    perror ("sem_post: mutex_sem"); exit (1);
        }
    
	// Produce una cadena
        sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);
	// Dice al spooler que hay una cadena para imprimir: V (spool_signal_sem);
        if (sem_post (spool_signal_sem) == -1) {
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
        // Hay un string para imprimir? P (spool_signal_sem);
        if (sem_wait (spool_signal_sem) == -1) {
	    perror ("sem_wait: spool_signal_sem"); exit (1);
        }
    
        printf ("%s", buf [buffer_print_index]);

        /* Desde que solamente hay un thread usando el
           buffer_print_index, mutex semaphore no es necesario */
        buffer_print_index++;
        if (buffer_print_index == MAX_BUFFERS)
           buffer_print_index = 0;

        /* contenido del buffer ha ser impreso
           Una vez mas esta disponible para los productores.
           Liberando el buffer: V (buffer_count_sem);  */
        if (sem_post (buffer_count_sem) == -1) {
	    perror ("sem_post: buffer_count_sem"); exit (1);
        }
    }
}
