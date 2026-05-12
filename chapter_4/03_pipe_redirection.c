#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pipe_fds[2];
    pipe(pipe_fds);

    if(fork() == 0){
        // -- 첫 번째 자식: ls -l (데이터를 보내는 쪽) -- 
        // 1. 자신의 표준 출력 -> pipe_fds[1](쓰기)로 교체
        dup2(pipe_fds[1], STDOUT_FILENO);

        // 2. 교체 후에는 기존 파이프 FD 닫기
        close(pipe_fds[0]);
        close(pipe_fds[1]);

        // 3. printf나 ls의 결과는 화면이 아닌 파이프로 전송
        execlp("ls", "ls", "-l", NULL);
    }

    if(fork() == 0){
        // -- 두 번째 자식: grep .c 실행(데이터를 받는쪽) -- 
        // 1. 자신의 표준 입력을 파이프 읽기로 교체
        dup2(pipe_fds[0], STDIN_FILENO);

        close(pipe_fds[0]);
        close(pipe_fds[1]);

        // 2. grep은 키보드가 아닌 파이프로부터 데이터를 받음
        execlp("grep", "grep", ".c", NULL);
    }

    close(pipe_fds[0]);
    close(pipe_fds[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}