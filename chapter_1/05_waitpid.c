#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void PrintProcessInfo(const char *name){
     printf("-------%s-------\nPID=%d\tPPID=%d\n", name, (int)getpid(), (int)getppid());
     fflush(stdout);
}

int main(int argc, char *argv[]){
    PrintProcessInfo("Parent");

    pid_t pid1 = fork();

    if(pid1 < 0){
        perror("fork faild\n");
        exit(1);
    }
    else if(pid1 == 0){
        PrintProcessInfo("Child_1");
        sleep(3);
        PrintProcessInfo("Child_1 - done(10)");
        exit(10);
    }

    pid_t pid2 = fork();
    if(pid2 < 0){
        perror("fork failed\n");
        exit(1);
    }
    else if(pid2 == 0){
        PrintProcessInfo("Child_2");
        sleep(1);
        PrintProcessInfo("Child_2 - done(20)");
        exit(20);
    }

    int status;
    pid_t wpid;

    wpid = waitpid(pid2, &status, 0);
    printf("waitpid() returned: %d\n", wpid);
    if(WIFEXITED(status)){
        printf("Child_2 exited with: %d\n", WEXITSTATUS(status));
    }

    wpid = wait(&status);
    printf("wiat() returned: %d\n", wpid);
    if(WIFEXITED(status)){
        printf("Child_1 exited with: %d\n", WEXITSTATUS(status));
    }

    PrintProcessInfo("Parent done");

    return 0;
}