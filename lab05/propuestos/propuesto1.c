#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long tipo;
    int info;
} MENSAJE;

int main(void) {
    setbuf(stdout, NULL);
    int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (qid == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }
    printf("Cola creada: %d\n", qid);
    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        msgctl(qid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (hijo == 0) {
        MENSAJE recibido;
        if (msgrcv(qid, &recibido, sizeof(recibido.info), 7, 0) == -1) {
            perror("msgrcv");
            _exit(EXIT_FAILURE);
        }
        printf("Hijo %ld recibe tipo = %ld con info = %d\n",
               (long)getpid(), recibido.tipo, recibido.info);
        _exit(EXIT_SUCCESS);
    }
    MENSAJE mensaje = {7, 150};
    printf("Padre %ld envia tipo = %ld con info = %d\n",
           (long)getpid(), mensaje.tipo, mensaje.info);
    int fallo = 0;
    if (msgsnd(qid, &mensaje, sizeof(mensaje.info), 0) == -1) {
        perror("msgsnd");
        fallo = 1;
        msgctl(qid, IPC_RMID, NULL);
    }
    int estado;
    if (waitpid(hijo, &estado, 0) == -1) {
        perror("waitpid");
        fallo = 1;
    } else if (!WIFEXITED(estado) || WEXITSTATUS(estado) != 0) {
        fallo = 1;
    }
    if (!fallo && msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return EXIT_FAILURE;
    }
    if (fallo) {
        msgctl(qid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    puts("Cola eliminada; ambos procesos finalizaron.");
    return EXIT_SUCCESS;
}
