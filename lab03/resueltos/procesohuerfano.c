// Un programa en C para demostrar el proceso huérfano.
// El proceso principal finaliza la ejecución mientras el
// el proceso hijo se está ejecutando. El proceso hijo
// queda huérfano
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // Creando un proceso hijo
    int pid = fork();

    if (pid > 0)
        printf("in proceso padre");
    // Note que el pid es 0 en un proceso hijo
    // y negativo si fork() falla
    else if (pid == 0)
    {
        sleep(30);
        printf("in proceso hijo");
    }

    return 0;
}
