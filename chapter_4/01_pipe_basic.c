#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(){
    int pipe_fds[2]; // 파이프용 FD 두 개(0: 읽기, 1: 쓰기)
    char buf[30];
    char *msg = "Hello from Parent";

    if(pipe(pipe_fds) == -1){
        perror("pipe 생성 실패");
        return 1;
    }

    printf("[파이프 생성 완료]. Read FD: %d, Write FD: %d\n", pipe_fds[0], pipe_fds[1]);

    pid_t pid = fork();

    if(pid == 0){
        close(pipe_fds[1]); // 사용하지 않는 쓰기용 FD 닫음

        printf("[자식] 부모로부터 메시지를 기다리는 중..\n");
        read(pipe_fds[0], buf, sizeof(buf));

        printf("[자식] 받은 메시지: %s\n", buf);
        close(pipe_fds[0]);
    }
    else{
        close(pipe_fds[0]);

        printf("[부모] 자식에게 메시지를 보냄\n");
        write(pipe_fds[1], msg, strlen(msg)+1);

        close(pipe_fds[1]);
        printf("[부모] 전송 완료\n");
    }
    return 0;
}
