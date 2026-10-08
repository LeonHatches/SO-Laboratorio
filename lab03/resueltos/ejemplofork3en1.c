#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pid, pid1, pid2;

    // la variable pid alojara el
    // valor retornado desde fork()
    pid = fork();

    // If fork() retorna cero entonces eso
    // signifca que es el proceso hijo.
    if (pid == 0) {
        // El primer hijo necesita ser impreso
        // después se hace que este proceso duerma
        // por 3 segundos.
        sleep(3);

        // Este es el primer proceso hijo
        // getpid() proporciona el id del procesos
        // y getppid() da el id del padre
        // de este proceso.
        printf("child[1] --> pid = %d and ppid = %d\n",
               getpid(), getppid());
    }
    else {
        pid1 = fork();
        if (pid1 == 0) {
            sleep(2);
            printf("child[2] --> pid = %d and ppid = %d\n",
                   getpid(), getppid());
        }
        else {
            pid2 = fork();
            if (pid2 == 0) {
                // Este es el tercer hijo el cual es
                // necesitado para ser impreso primero.
                printf("child[3] --> pid = %d and ppid = %d\n",
                       getpid(), getppid());
            }
            // Si el valor retornado desde fork()
            // es no cero y >0 eso significa
            // este es el proceso padre.
            else {
                // Esto es cuestionado para ser impreso al final
                // es hecho dormir por 3 segundos.
                sleep(3);
                printf("parent --> pid = %d\n", getpid());
            }
        }
    }

    return 0;
}
