#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 100000

long long shared_count = 0;

void *thread_func(void *arg){
    for(int i = 0; i < ITERATIONS; i++){
        shared_count++; // 3번의 동작으로 나눠서 연산됨
    }
    return NULL;
}

int main(){
    printf("[시작] 초기값: %lld, 목표값: %d\n", shared_count, ITERATIONS*2);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_func, NULL);
    pthread_create(&thread2, NULL, thread_func, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("최종 결과: %lld\n", shared_count);

    if(shared_count == ITERATIONS *2){
        printf("결과: 정확\n");
    }
    else{
        printf("결과: 데이터 유실(차이 %lld)\n", (ITERATIONS*2) - shared_count);
    }

    return 0;
}
