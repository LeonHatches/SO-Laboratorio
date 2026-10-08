#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
        printf("HC: Hola desde el hijo\n");
    else
    {
        printf("HP: hola desde el padre\n");
        wait(NULL);
        printf("CT: el hijo ha terminado\n");
    }

    printf("Bye\n");
    return 0;
}
