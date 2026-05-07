#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int val = 100;

    printf(" -- fork 실행 전 --\n");
    printf("초기 val: %d | 주소: %p\n", val, (void*)&val);

    pid_t pid = fork();

    if(pid == 0){
        printf(" -- [자식] --\n");
        val = 200;
        printf("수정 후 val%d | 주소: %p\n", val, (void*)&val);
    }
    else{
        sleep(1);
        printf(" -- [부모] --\n");
        printf("현재 val: %d | 주소: %p\n", val, (void*)&val);
    }
}