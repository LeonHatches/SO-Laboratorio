#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // hace dos procesos los cuales corren en el mismo
    // programa después de esta instrucción
    fork();

    printf("Hola mundo!\n");
    return 0;
}
