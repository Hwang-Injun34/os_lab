#include <stdio.h>
#include <pthread.h>
#define ITERATIONS 100000

long long shared_count = 0;
pthread_mutex_t lock; 


void *thread_func(void *arg){
    for(int i = 0; i < ITERATIONS; i++){
        
        pthread_mutex_lock(&lock);
        shared_count++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    printf("[시작] 초기값: %lld\n", shared_count);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_func, NULL);
    pthread_create(&thread2, NULL, thread_func, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 뮤텍스 제거
    pthread_mutex_destroy(&lock);

    printf("최종 결과: %lld\n", shared_count);

    if(shared_count == ITERATIONS * 2){
        printf("결과: 정확\n");
    }
    else {
        printf("결과: 오염 %lld\n", ITERATIONS * 2 - shared_count);
    }

    return 0;
}