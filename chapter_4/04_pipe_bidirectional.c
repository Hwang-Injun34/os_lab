#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    int p2c[2], c2p[2]; // 두 개의 파이프 배열
    char buf[100];

    // 1. 파이프 두 개 생성
    if(pipe(p2c) < 0 || pipe(c2p) < 0) {
        perror("pipe 생성 실패");
        return 1;
    }

    pid_t pid = fork();

    if(pid == 0){
        close(p2c[1]); // 부모꺼 - 부모의 쓰기용 닫음
        close(c2p[0]); // 자식꺼 - 자식의 읽기용 닫음(내꺼 내가 읽을일 없음)

        // 부모의 질문 읽기
        read(p2c[0], buf, sizeof(buf));
        printf("[자식] 질문 받음: %s\n", buf);

        // 부모에게 답변 보내기
        char *answer = "제 이름은 자식 프로세스입니다.";
        write(c2p[1], answer, strlen(answer)+ 1);

        close(p2c[0]);
        close(c2p[1]);
    }
    else {
        close(p2c[0]); // 부모꺼 - 부모의 읽기용 닫음
        close(c2p[1]); // 자식꺼 - 자식의 쓰기용 닫음

        // 자식에게 질문 보내기
        char *question = "너의 이름은 무엇입니까?";
        write(p2c[1], question, strlen(question)+1);

        // 자식 답변 기다리기
        read(c2p[0], buf, sizeof(buf));
        printf("[부모] 답변 받음: %s\n", buf);

        close(p2c[1]);
        close(c2p[0]);
    }
    return 0;

}