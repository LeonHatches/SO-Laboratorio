#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char tablero[3][3];
    char resultado;
} PARTIDA;

typedef struct {
    long tipo;
    int fin;
} AVISO;

static void enviar(int qid, long tipo, int fin) {
    AVISO aviso = {tipo, fin};
    while (msgsnd(qid, &aviso, sizeof(aviso.fin), 0) == -1) {
        if (errno == EINTR) continue;
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

static AVISO recibir(int qid, long tipo) {
    AVISO aviso;
    while (msgrcv(qid, &aviso, sizeof(aviso.fin), tipo, 0) == -1) {
        if (errno == EINTR) continue;
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    return aviso;
}

static void mostrar(const PARTIDA *p) {
    for (int f = 0; f < 3; f++) {
        printf(" %c | %c | %c\n", p->tablero[f][0],
               p->tablero[f][1], p->tablero[f][2]);
        if (f < 2) puts("---+---+---");
    }
}

static char resultado(const PARTIDA *p) {
    const char (*t)[3] = p->tablero;
    for (int i = 0; i < 3; i++) {
        if (t[i][0] != ' ' && t[i][0] == t[i][1] && t[i][1] == t[i][2])
            return t[i][0];
        if (t[0][i] != ' ' && t[0][i] == t[1][i] && t[1][i] == t[2][i])
            return t[0][i];
    }
    if (t[0][0] != ' ' && t[0][0] == t[1][1] && t[1][1] == t[2][2])
        return t[0][0];
    if (t[0][2] != ' ' && t[0][2] == t[1][1] && t[1][1] == t[2][0])
        return t[0][2];
    for (int f = 0; f < 3; f++)
        for (int c = 0; c < 3; c++)
            if (t[f][c] == ' ') return ' ';
    return '=';
}

static void jugar(int qid, PARTIDA *p, int jugador) {
    int otro = 3 - jugador;
    char ficha = jugador == 1 ? 'X' : 'O';
    int primero = jugador == 1;
    for (;;) {
        if (!primero) {
            AVISO turno = recibir(qid, jugador);
            enviar(qid, jugador + 10, 0);
            if (turno.fin) break;
        }
        primero = 0;
        mostrar(p);
        char linea[128];
        for (;;) {
            printf("Proceso %ld, jugador %c. Fila y columna (1-3): ",
                   (long)getpid(), ficha);
            if (fgets(linea, sizeof(linea), stdin) == NULL) {
                p->resultado = '!';
                break;
            }
            int f, c;
            char extra;
            if (sscanf(linea, "%d %d %c", &f, &c, &extra) != 2 ||
                f < 1 || f > 3 || c < 1 || c > 3 ||
                p->tablero[f - 1][c - 1] != ' ') {
                puts("Jugada invalida. El turno no cambia.");
                continue;
            }
            p->tablero[f - 1][c - 1] = ficha;
            p->resultado = resultado(p);
            break;
        }
        int fin = p->resultado != ' ';
        printf("%c entrega turno a %c y espera confirmacion.\n",
               ficha, otro == 1 ? 'X' : 'O');
        enviar(qid, otro, fin);
        recibir(qid, otro + 10);
        printf("%c recibe confirmacion.\n", ficha);
        if (fin) break;
    }
}

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    int shmid = shmget(IPC_PRIVATE, sizeof(PARTIDA), IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    PARTIDA *p = shmat(shmid, NULL, 0);
    if (p == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    memset(p->tablero, ' ', sizeof(p->tablero));
    p->resultado = ' ';
    int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (qid == -1) {
        perror("msgget");
        shmdt(p);
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    printf("Cola: %d; memoria compartida: %d\n", qid, shmid);
    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        msgctl(qid, IPC_RMID, NULL);
        shmdt(p);
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    jugar(qid, p, hijo == 0 ? 2 : 1);
    if (hijo == 0) {
        shmdt(p);
        return EXIT_SUCCESS;
    }
    int estado;
    pid_t esperado;
    do { esperado = waitpid(hijo, &estado, 0); }
    while (esperado == -1 && errno == EINTR);
    mostrar(p);
    if (p->resultado == '=') puts("Empate.");
    else if (p->resultado == '!') puts("Partida terminada por fin de entrada.");
    else printf("Gana %c.\n", p->resultado);
    int fallo = esperado == -1;
    if (!fallo) fallo = !WIFEXITED(estado) || WEXITSTATUS(estado) != 0;
    if (msgctl(qid, IPC_RMID, NULL) == -1) fallo = 1;
    if (shmdt(p) == -1) fallo = 1;
    if (shmctl(shmid, IPC_RMID, NULL) == -1) fallo = 1;
    return fallo ? EXIT_FAILURE : EXIT_SUCCESS;
}
