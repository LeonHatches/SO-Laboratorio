#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    char directorio[] = "/tmp/lab04_fifo_XXXXXX";
    char ruta[100];
    if (mkdtemp(directorio) == NULL) {
        perror("mkdtemp");
        return EXIT_FAILURE;
    }
    snprintf(ruta, sizeof(ruta), "%s/datos", directorio);
    if (mkfifo(ruta, 0600) == -1) {
        perror("mkfifo");
        rmdir(directorio);
        return EXIT_FAILURE;
    }
    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        unlink(ruta);
        rmdir(directorio);
        return EXIT_FAILURE;
    }
    if (hijo == 0) {
        int fd = open(ruta, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return EXIT_FAILURE;
        }
        char buffer[100];
        size_t total = 0;
        while (total < sizeof(buffer) - 1) {
            ssize_t leidos = read(fd, buffer + total, sizeof(buffer) - 1 - total);
            if (leidos == -1 && errno == EINTR)
                continue;
            if (leidos == -1) {
                perror("read");
                close(fd);
                return EXIT_FAILURE;
            }
            if (leidos == 0)
                break;
            total += (size_t)leidos;
        }
        buffer[total] = '\0';
        close(fd);
        printf("Hijo: recibido por FIFO \"%s\"\n", buffer);
        return EXIT_SUCCESS;
    }
    int fd = open(ruta, O_WRONLY);
    int correcto = fd != -1;
    if (!correcto) {
        perror("open");
    } else {
        const char mensaje[] = "Sistemas Operativos";
        ssize_t escritos;
        do {
            escritos = write(fd, mensaje, sizeof(mensaje));
        } while (escritos == -1 && errno == EINTR);
        correcto = escritos == (ssize_t)sizeof(mensaje);
        if (!correcto)
            perror("write");
        close(fd);
    }
    int estado;
    pid_t resultado;
    do {
        resultado = waitpid(hijo, &estado, 0);
    } while (resultado == -1 && errno == EINTR);
    unlink(ruta);
    rmdir(directorio);
    return correcto && resultado != -1 && WIFEXITED(estado) &&
           WEXITSTATUS(estado) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
