#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int n;
    printf("Numero de hijos: ");
    fflush(stdout);

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Ingrese un entero positivo.\n");
        return EXIT_FAILURE;
    }

    pid_t *hijos = malloc((size_t)n * sizeof(*hijos));
    if (hijos == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    int creados = 0;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) {
            printf("Hijo %d: PID=%ld, PPID=%ld\n",
                   i + 1, (long)getpid(), (long)getppid());
            free(hijos);
            return EXIT_SUCCESS;
        }

        hijos[creados++] = pid;
    }

    int resultado = creados == n ? EXIT_SUCCESS : EXIT_FAILURE;
    for (int i = 0; i < creados; i++) {
        if (waitpid(hijos[i], NULL, 0) == -1) {
            perror("waitpid");
            resultado = EXIT_FAILURE;
        }
    }

    printf("Padre: PID=%ld, hijos creados=%d\n", (long)getpid(), creados);
    free(hijos);
    return resultado;
}
