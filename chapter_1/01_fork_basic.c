#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf(" -- Start -- (PID: %d)\n", getpid());
    
    pid_t pid = fork();
    printf(" -- Hello --(PID: %d)\n", getpid());

    sleep(10);
    return 0;
}