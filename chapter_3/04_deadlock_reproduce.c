#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 두 개의 자원 준비
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER; 

void *thread_A(void *arg){
    printf("[스레드 A] 1번 열쇠 잡기");
    pthread_mutex_lock(&lock1);
    printf("[스레드 A] 1번 열쇠 획득: 2번 열쇠 잡으러 가기");

    sleep(1);

    pthread_mutex_lock(&lock2);
    printf("[스레드 A] 2번 열쇠까지 획득\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void *thread_B(void *arg){
    printf("[스레드 B] 2번 열쇠 잡기");
    pthread_mutex_lock(&lock2);
    printf("[스레드 B] 2번 열쇠 획득: 1.번 열쇠 잡으러 가기");

    sleep(1);

    pthread_mutex_lock(&lock1);
    printf("[스레드 B] 1번 열쇠까지 획득\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main(){
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_A, NULL);
    pthread_create(&t2, NULL, thread_B, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("프로그램이 정상 종료\n");
    return 0;
}