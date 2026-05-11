#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int global_val = 10; 

void *thread_main(void *arg){
    global_val += 5;
    printf("[자식 스레드] 전역 변수 수정 (+5). 현재값: %d, 주소: %p\n", global_val, (void*)&global_val);
    return NULL;
}

int main(){
    printf("-- [시작] 초기 전역 변수 값: %d, 주소: %p -- \n\n ", global_val, (void*)&global_val);

    // printf("[프로세스 시작]\n");
    // pid_t pid = fork();

    // if(pid == 0){
    //     global_val += 20;
    //     printf("[자식 프로세스] 전역 변수 수정 (+20). 현재값: %d, 주소: %p\n", global_val, (void*)&global_val);
    //     exit(0);
    // }
    // else{
    //     wait(NULL);
    //     printf("[부모 프로세스] 자식 종료 후 현재값: %d, 주소: %p\n", global_val, (void*)&global_val);
    // }

    printf("[스레드 시작]\n");
    pthread_t tid;

    if(pthread_create(&tid, NULL, thread_main, NULL) != 0){
        perror("thread 생성 실패");
        return 1;
    }
    pthread_join(tid, NULL); // 스레드가 끝날 때까지 대기
    printf("[메인 스레드] 자식 스레드 종료 후 현재값: %d, 주소: %p\n", global_val, (void*)&global_val);
    
    return 0;
    
}