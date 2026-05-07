#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define SIZE 1024 * 1024 * 100 

int data[SIZE] = {0};

int main(int argc, char *argv[]){
    
    printf("-- fork 실행 전 --\n");

    pid_t pid = fork();

    if(pid == 0){
        printf("-- [자식] (PID: %d) --\n", (int)getpid());
        printf(" 데이터 읽기 ...\n");
        int temp = data[0];
        printf(" 데이터 수정 ...\n");
        data[0] = 777;
        printf(" 수정 완료 ...\n");
    }
    else{
        sleep(30);
        printf("-- [부모] --\n");
        printf("데이터: %d\n", data[0]);
    }
    return 0;
}