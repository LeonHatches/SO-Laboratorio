#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void forkexample()
{
    // es el proceso hijo porque retorna el valor cero
    if (fork() == 0)
        printf("Hola desde el hijo!\n");
    // es el proceso padre porque retorna un valor distinto a cero.
    else
        printf("Hola desde el padre!\n");
}

int main()
{
    forkexample();
    return 0;
}
