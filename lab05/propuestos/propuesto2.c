#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long tipo;
    char texto[64];
} MENSAJE;

static int fallar(const char *operacion, int qid) {
    perror(operacion);
    msgctl(qid, IPC_RMID, NULL);
    return EXIT_FAILURE;
}

int main(void) {
    int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (qid == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }
    MENSAJE mensaje = {1, "Hola desde IPC_NOWAIT"};
    MENSAJE recibido;
    ssize_t n = msgrcv(qid, &recibido, sizeof(recibido.texto),
                      1, IPC_NOWAIT);
    if (n != -1 || errno != ENOMSG) {
        puts("No se obtuvo el ENOMSG esperado.");
        msgctl(qid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    puts("Cola vacia: ENOMSG; el proceso no se bloquea.");
    if (msgsnd(qid, &mensaje, sizeof(mensaje.texto), IPC_NOWAIT) == -1)
        return fallar("msgsnd", qid);
    if (msgrcv(qid, &recibido, sizeof(recibido.texto),
               1, IPC_NOWAIT) == -1)
        return fallar("msgrcv", qid);
    printf("Lectura disponible: %s\n", recibido.texto);
    struct msqid_ds datos;
    if (msgctl(qid, IPC_STAT, &datos) == -1)
        return fallar("msgctl IPC_STAT", qid);
    datos.msg_qbytes = sizeof(mensaje.texto);
    if (msgctl(qid, IPC_SET, &datos) == -1)
        return fallar("msgctl IPC_SET", qid);
    if (msgsnd(qid, &mensaje, sizeof(mensaje.texto), IPC_NOWAIT) == -1)
        return fallar("msgsnd llenado", qid);
    int r = msgsnd(qid, &mensaje, sizeof(mensaje.texto), IPC_NOWAIT);
    if (r != -1 || errno != EAGAIN) {
        puts("No se obtuvo el EAGAIN esperado.");
        msgctl(qid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    puts("Cola llena: EAGAIN; el proceso no se bloquea.");
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        return EXIT_FAILURE;
    }
    puts("Cola eliminada.");
    return EXIT_SUCCESS;
}
