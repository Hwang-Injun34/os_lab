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
    printf("hello world(PID: %d)\n", (int)getpid());
    fflush(stdout);

    pid_t pid = fork();
    
    if(pid < 0){
        perror("fork failed\n");
        exit(1);
    }
    else if(pid == 0){
        PrintProcessInfo("Child");
        sleep(3);
        PrintProcessInfo("Child is done");
        exit(42);
    }
    else{
        int status;
        printf("Parent waiting for Child ... \n");
        fflush(stdout);

        pid_t child_pid = wait(&status);

        printf("Child %d finished\n", child_pid);
        if(WIFEXITED(status)){
            printf("Child exited with status: %d\n", WEXITSTATUS((status)));
        }
        PrintProcessInfo("Parent");
    }

    return 0;
}