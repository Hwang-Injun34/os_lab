#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void PrintProcessInfo(const char *name){
     printf("-------%s-------\nPID=%d\tPPID=%d\n", name, (int)getpid(), (int)getppid());
     fflush(stdout);
}


int main(int argc, char *argv[]){
    PrintProcessInfo("Parent");

    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed");
        exit(1);
    }
    else if(pid == 0){
        PrintProcessInfo("Child");
        
        // execute list : 실행 파일 경로, 실행할 프로그램 이름, 인자의 마지막으로 NULL 표시
        execl("/bin/ls", "ls", NULL);
        perror("execl failed");

        exit(1);
    }
    else{
        int status;
        wait(&status);
        
        if(WIFEXITED(status)){
        printf("Child_1 exited with: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}