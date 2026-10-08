#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include<wait.h>

#include<signal.h>
int main() {
int stat;
pid_t pid;
pid = fork();
if (pid == -1) {
perror("fork");
return EXIT_FAILURE;
}
if (pid == 0)
while(1) ;
else {
kill(pid, SIGINT);
wait(&stat);
if (WIFSIGNALED(stat))
psignal(WTERMSIG(stat), "el hijo termino debido a:");
}
}
