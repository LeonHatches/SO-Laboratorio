#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int enviar(int fd, const char *mensaje, size_t longitud)
{
    size_t total = 0;
    while (total < longitud) {
        ssize_t escritos = write(fd, mensaje + total, longitud - total);
        if (escritos == -1 && errno == EINTR)
            continue;
        if (escritos <= 0)
            return -1;
        total += (size_t)escritos;
    }
    return 0;
}

static int recibir(int fd, char *buffer, size_t capacidad)
{
    size_t total = 0;
    while (total + 1 < capacidad) {
        ssize_t leidos = read(fd, buffer + total, capacidad - total - 1);
        if (leidos == -1 && errno == EINTR)
            continue;
        if (leidos == -1)
            return -1;
        if (leidos == 0)
            break;
        total += (size_t)leidos;
    }
    buffer[total] = '\0';
    return 0;
}

int main(void)
{
    int ida[2], vuelta[2];
    char buffer[100];
    if (pipe(ida) == -1 || pipe(vuelta) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }
    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (hijo == 0) {
        close(ida[1]);
        close(vuelta[0]);
        if (recibir(ida[0], buffer, sizeof(buffer)) == -1) {
            perror("read");
            return EXIT_FAILURE;
        }
        close(ida[0]);
        printf("Hijo: recibido \"%s\"\n", buffer);
        if (enviar(vuelta[1], "Operativos", sizeof("Operativos")) == -1) {
            perror("write");
            return EXIT_FAILURE;
        }
        close(vuelta[1]);
        return EXIT_SUCCESS;
    }
    close(ida[0]);
    close(vuelta[1]);
    printf("Padre: envio \"Sistemas\"\n");
    fflush(stdout);
    if (enviar(ida[1], "Sistemas", sizeof("Sistemas")) == -1) {
        perror("write");
        return EXIT_FAILURE;
    }
    close(ida[1]);
    if (recibir(vuelta[0], buffer, sizeof(buffer)) == -1) {
        perror("read");
        return EXIT_FAILURE;
    }
    close(vuelta[0]);
    printf("Padre: recibido \"%s\"\n", buffer);
    int estado;
    pid_t resultado;
    do {
        resultado = waitpid(hijo, &estado, 0);
    } while (resultado == -1 && errno == EINTR);
    if (resultado == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    return WIFEXITED(estado) && WEXITSTATUS(estado) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
