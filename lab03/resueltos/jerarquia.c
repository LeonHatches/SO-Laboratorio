#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    pid_t pid1, pid2;
    int status1, status2;

    if ((pid1 = fork()) == 0)
    { /* hijo (1a generacion) = padre */
        if ((pid2 = fork()) == 0)
        { /* hijo (2a generacion) = nieto */
            printf("Soy el nieto (%d, hijo de %d)\n", getpid(), getppid());
        }
        else
        { /* padre (2a generacion) = padre */
            wait(&status2);
            printf("Soy el padre (%d, hijo de %d)\n", getpid(), getppid());
        }
    }
    else
    { /* padre (1a generacion) = abuelo */
        wait(&status1);
        printf("Soy el abuelo (%d, hijo de %d)\n", getpid(), getppid());
    }

    return 0;
}
