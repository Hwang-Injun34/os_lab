#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf("hello world(PID: %d)\n", (int)getpid());
    int x = 10;

    pid_t pid = fork();

    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(pid == 0){
        printf("--Child Process--\n");
        printf("Child (PID: %d)\n", getpid());
        printf("Parent (PID: %d)\n", getppid());

        x++;
        printf("Child (x: %d)\n", x);
    }
    else {
        sleep(1);
        printf("--Parent Process--\n");
        printf("Parent (PID: %d)\n", getpid());
        printf("Child (PID: %d)\n", pid);

        printf("Parent (x: %d)\n", x);
    }

    return 0;
}
