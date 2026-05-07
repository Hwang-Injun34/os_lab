#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    pid_t pid = fork();

    if(pid == 0){
        for(int i = 0; i < 100; i++){
                printf("C");
                fflush(stdout);
                usleep(100);
        }
    }
    else {
        for(int i= 0; i < 100; i++){
                printf("P");
                fflush(stdout);
                usleep(100);
        }
    }

    printf("\n");
    return 0;
}
