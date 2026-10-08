#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    const char *nombres[] = {"Bisabuelo", "Abuelo", "Padre", "Nieto"};
    int nivel = 0;

    while (nivel < 3) {
        pid_t hijo = fork();

        if (hijo == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (hijo > 0) {
            printf("%s: PID=%ld, PPID=%ld, hijo=%ld\n",
                   nombres[nivel], (long)getpid(),
                   (long)getppid(), (long)hijo);

            if (waitpid(hijo, NULL, 0) == -1) {
                perror("waitpid");
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }

        nivel++;
    }

    printf("%s: PID=%ld, PPID=%ld\n",
           nombres[nivel], (long)getpid(), (long)getppid());
    return EXIT_SUCCESS;
}
