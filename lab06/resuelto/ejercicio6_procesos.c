#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *fin;
    long n = argc == 2 ? strtol(argv[1], &fin, 10) : 100;
    if (argc > 2 || (argc == 2 && (fin == argv[1] || *fin)) ||
        n < 1 || n > 100000) {
        fputs("Indica una cantidad de 1 a 100000.\n", stderr);
        return EXIT_FAILURE;
    }

    struct timespec inicio, fin_medicion;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (long i = 0; i < n; i++) {
        pid_t hijo = fork();
        if (hijo == 0) _exit(0);
        if (hijo < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }
        pid_t esperado;
        do {
            esperado = waitpid(hijo, NULL, 0);
        } while (esperado < 0 && errno == EINTR);
        if (esperado < 0) {
            perror("waitpid");
            return EXIT_FAILURE;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &fin_medicion);
    double us = (fin_medicion.tv_sec - inicio.tv_sec) * 1e6
              + (fin_medicion.tv_nsec - inicio.tv_nsec) / 1e3;
    printf("%ld procesos: %.2f us en promedio\n", n, us / n);
    return EXIT_SUCCESS;
}
