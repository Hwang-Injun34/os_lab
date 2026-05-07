#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t pid = fork();

    if(pid == 0){
        printf("[자식] PID: %d\n", getpid());
        exit(0);
    }
    else{
        printf("[부모]\n");
        printf("ps -ef | grep 07.out\n");
        sleep(20);

        wait(NULL); // 좀비 해제
        sleep(10);
        printf("종료\n");
    }
    return 0;
}