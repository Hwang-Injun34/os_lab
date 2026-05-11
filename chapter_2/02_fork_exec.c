#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    printf("[부모] PID: %d\n", getpid());

    pid_t pid = fork();

    if(pid < 0){
        perror("fork 실패");
        return 0;
    }
    else if(pid == 0){
        printf("[자식] PID: %d\n", getpid());

        execl("/bin/date", "date", NULL);

        perror("exec 실패");
        return 1;
    }
    else {
        printf("[부모]a\n");
        wait(NULL);
        printf("[부모] - 자식 종료\n");
    }
    return 0;
}