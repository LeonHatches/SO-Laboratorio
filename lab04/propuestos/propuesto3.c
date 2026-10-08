#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t recibida = 0;

static void recibir(int senal)
{
    (void)senal;
    recibida = 1;
}

static int esperar(pid_t hijo)
{
    int estado;
    pid_t resultado;
    do {
        resultado = waitpid(hijo, &estado, 0);
    } while (resultado == -1 && errno == EINTR);
    return resultado != -1 && WIFEXITED(estado) && WEXITSTATUS(estado) == 0;
}

int main(int argc, char *argv[])
{
    long repeticiones = 3;
    if (argc > 3) {
        fprintf(stderr, "Uso: %s [repeticiones] [script]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc >= 2) {
        char *fin;
        errno = 0;
        repeticiones = strtol(argv[1], &fin, 10);
        if (errno != 0 || fin == argv[1] || *fin != '\0' ||
            repeticiones < 1 || repeticiones > INT_MAX) {
            fprintf(stderr, "Las repeticiones deben ser un entero positivo.\n");
            return EXIT_FAILURE;
        }
    }
    const char *script = argc == 3 ? argv[2] : "./bienvenida.sh";
    setbuf(stdout, NULL);

    sigset_t bloqueadas, anterior;
    sigemptyset(&bloqueadas);
    sigaddset(&bloqueadas, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &bloqueadas, &anterior) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    pid_t h2 = fork();
    if (h2 == -1) {
        perror("fork H2");
        return EXIT_FAILURE;
    }
    if (h2 == 0) {
        struct sigaction accion = {0};
        accion.sa_handler = recibir;
        sigemptyset(&accion.sa_mask);
        if (sigaction(SIGUSR1, &accion, NULL) == -1) {
            perror("sigaction");
            return EXIT_FAILURE;
        }
        sigset_t espera = anterior;
        sigdelset(&espera, SIGUSR1);
        for (long i = 0; i < repeticiones; i++) {
            while (!recibida)
                sigsuspend(&espera);
            recibida = 0;
            printf("H2: PID=%ld, recibida SIGUSR1 (%ld/%ld)\n",
                   (long)getpid(), i + 1, repeticiones);
        }
        return EXIT_SUCCESS;
    }

    pid_t h1 = fork();
    if (h1 == -1) {
        perror("fork H1");
        kill(h2, SIGTERM);
        esperar(h2);
        return EXIT_FAILURE;
    }
    if (h1 == 0) {
        sigprocmask(SIG_SETMASK, &anterior, NULL);
        printf("H1: PID=%ld, PPID=%ld\n", (long)getpid(), (long)getppid());
        pid_t h3 = fork();
        if (h3 == -1) {
            perror("fork H3");
            return EXIT_FAILURE;
        }
        if (h3 == 0) {
            execl("/bin/sh", "sh", script, (char *)NULL);
            perror("execl");
            return EXIT_FAILURE;
        }
        if (!esperar(h3))
            return EXIT_FAILURE;

        for (long i = 0; i < repeticiones; i++) {
            struct timespec restante = {20, 0};
            while (nanosleep(&restante, &restante) == -1) {
                if (errno != EINTR) {
                    perror("nanosleep");
                    return EXIT_FAILURE;
                }
            }
            if (kill(h2, SIGUSR1) == -1) {
                perror("kill");
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    sigprocmask(SIG_SETMASK, &anterior, NULL);
    printf("P: PID=%ld, H1=%ld, H2=%ld\n", (long)getpid(), (long)h1, (long)h2);
    int correcto_h1 = esperar(h1);
    if (!correcto_h1)
        kill(h2, SIGTERM);
    int correcto_h2 = esperar(h2);
    printf("P: sus hijos han terminado.\n");
    return correcto_h1 && correcto_h2 ? EXIT_SUCCESS : EXIT_FAILURE;
}
