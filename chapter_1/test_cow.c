#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE (1024 * 64) // 64KB

int main(int argc, char *argv[]) {
    // 1. 메모리 할당
    char *arr = malloc(SIZE);

    // 2. 부모가 메모리를 실제로 사용 
    memset(arr, 'A', SIZE);

    printf("Parent PID: %d\n", getpid());
    printf("Private_Dirty 확인하기,  - 계속 하려면 엔터...\n");
    getchar();

    pid_t pid = fork();

    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(pid == 0){
        printf("\n[Child] PID: %d\n", getpid());
        printf("[Child] 쓰기 전, 계속 하려면 엔터...\n");
        getchar();

        // 자식이 메모리 수정
        memset(arr, 'B', SIZE);
        printf("[Child] 쓰기 완료\n");
    
        while(1) sleep(1);
    }
    else{
        while(1) sleep(1);    
    }

    return 0;
}