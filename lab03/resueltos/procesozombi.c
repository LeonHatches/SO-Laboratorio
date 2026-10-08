// Un programa en C para demostrar el proceso zombi
// El hijo comienza a ser zombi cuando el padre duerme
// cuando el proceso hijo sale.
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // Fork retorna el proceos id
    // en el proceso padre
    pid_t child_pid = fork();

    // Proceso padre
    if (child_pid > 0)
        sleep(50);
    // Proceso Hijo
    else
        exit(0);

    return 0;
}
