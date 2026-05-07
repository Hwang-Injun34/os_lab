#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    printf("-- hello -- (PID: %d)\n", getpid());

    pid_t pid = fork();

    if(pid < 0){
        perror("fork error");
        return 1;
    }
    else if(pid == 0){
        printf("-- [자식] --\n");
        printf("내 PID: %d\n", getpid());
        printf("내 부모 PID: %d\n", getppid());
        printf("fork 리턴값: %d\n", pid);
        sleep(30);
    }
    else {
        printf("-- [부모] --\n");
        printf("내 PID: %d\n", getpid());
        printf("내 자식 PID: %d\n", pid);    
        printf("fork 리턴값: %d\n", pid);
        sleep(30);
    }
    return 0;
}