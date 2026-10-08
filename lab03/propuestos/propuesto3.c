#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t hijo = fork();

    if (hijo == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (hijo == 0) {
        printf("Hijo: PID=%ld, PPID=%ld\n", (long)getpid(), (long)getppid());
        return EXIT_SUCCESS;
    }

    int estado;
    pid_t resultado;

    do {
        resultado = waitpid(hijo, &estado, 0);
    } while (resultado == -1 && errno == EINTR);

    if (resultado == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    printf("Padre: PID=%ld, hijo %ld recolectado\n",
           (long)getpid(), (long)resultado);

    if (WIFEXITED(estado))
        printf("Estado de salida del hijo: %d\n", WEXITSTATUS(estado));

    return EXIT_SUCCESS;
}
