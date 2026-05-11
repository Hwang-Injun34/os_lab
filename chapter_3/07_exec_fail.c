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

        execl("/bin/nonexistent", "nonexistent", NULL);
        perror("execl failed");
        exit(1);
    }
    else{
        int status;
        pid_t child_pid = wait(&status);

        PrintProcessInfo("Parent End");
        if(WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        else {
            printf("Child did not exit normally\n");
        }
    }
    return 0;
}