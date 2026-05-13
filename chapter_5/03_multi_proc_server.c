#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>


// 좀비 프로세스 방지를 위한 시그널 핸들러
void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    // -1: 아무 자식 프로세스나 기다림
    // &status: 종료 상태 저장
    // WNOHANG: 자식이 종료되지 않았으면 블로킹하지말고 리턴/있으면 바로 리턴

    if (id > 0) printf("[커널] 자식 프로세스 %d 회수 완료\n", id);
}

/*
struct sigaction {
    - sa_handler: 시그널을 처리할 기본적인 함수 포인터(시그널 처리기)
        - SIG_DFL(기본 동작)
        - SIG_IGN(무시)
        - 또는 사용자 정의 함수 주소를 지정
    
    - sa_sigaction: 상세 시그널 처리기
        - sa_flags: SA_SIGINFO가 설정되어 있을 때 사용되는 상세 시그널 처리기
        - 시그널 발생 원인, PID 등 부가 정보 받을 수 있음
    
    - sa_mask:시그널 처리 중 블록할 시그널 집합
        - 시그널 핸들러가 실행되는 동안 블록(지연)시킬 시그널 집합
        - 핸들러 실행 중 특정 시그널이 다시 들어오는 것을 방지할 때 사용
    
    - sa_flags: 시그널 처리 동작 수정 플래그
        - SA_SIGINFO: sa_sigaction을 사용하겠다고 설정
        - SA_RESTART: 시스템 콜이 시그널에 의해 중단되었을 때 자동으로 재시작
        - SA_NODEFER: 핸들러 실행 중에 해당 시그널이 다시 들어와도 블록하지 않음
*/


int main(){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;
    pid_t pid;

    // 좀비 프로세스 방지 설정
    struct sigaction act; // 어떤 함수 실행할지, 어떤 옵션 실행할지, 실행 중 어떤 시그널 막을지 저장
    act.sa_handler = read_childproc; // SIGCHLD 발생 시 read_childproc() 자동 실행
    sigemptyset(&act.sa_mask); // 시그널 헨들러 실행 중에 추가로 막을 시그널 목록을 초기화(추가로 막을 시그널 없음)
    act.sa_flags = 0; // 시그널 처리 옵션 - 특별한 옵션 없이 기본 동작 사용
    sigaction(SIGCHLD, &act, 0); 
    // sigaction: 시스템 프로그래밍에서 특정 시그널을 받았을 때 프로세스가 수행할 동작을 설정하거나 변경하는 함수
    // 첫 번째 인자: 동작을 설정할 시그널 번호
    // act: 새로운 시그널 동작을 설정할 구조체 포인터
    // 리턴값: 성공 시 0


    // --- 소켓 설정 ---
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9999);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) return 1;
    listen(serv_sock, 5);

    while(1) {
        adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
        if(clnt_sock == -1) continue;

        // 클라이언트가 올 때마다 새로운 자식을 복제
        pid = fork();

        if(pid == 0){
            close(serv_sock); // 자식은 리스닝 소켓 필요 없음
            char buf[1024];
            int str_len;
            while((str_len = read(clnt_sock, buf, sizeof(buf))) != 0)
                write(clnt_sock, buf, str_len);

            close(clnt_sock);
            printf("[자식] 클라이언트 서비스 종료 및 변신 해제\n");
            return 0;
        } else { // 부모 프로세스
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;

}