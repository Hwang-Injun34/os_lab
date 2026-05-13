#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h> // struct timeval을 위함

/*
초(s)/밀리초(m)/마이크로초(u)
struct timeval {
    - tv_sec: 초              1초
    - tv_usec: 마이크로초       1/1000,000,000초  
}
*/


int main(){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;
    struct timeval timeout; // 타임아웃 설정을 위한 구조체

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // --- 1. 타임아웃 설정 (5초) --- 
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // --- 2. 소켓 옵션 적용 (수신 타임아웃: SO_RCVTIMEO) ---- 
    setsockopt(serv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    /*
    int setsockopt(
        int sockfd,
        int level,
        int optname,
        const void *optval,
        socklen_t optlen
    );
    - serv_sock: 옵션을 적용할 소켓
    - SOL_SOCKET: 소켓 자체 레벨의 옵션 설정
        - SOL_SOCKET(소켓 수준), IPPROTO_TCP(TCP 수준), IPPROTO_IP(IP 수준) 등
    - SO_RCVTIMEO: 수신(receive) 타임아웃 옵션
        - recv(), read() 등이 데이터를 기다리는 최대 시간 설정
    - (char *)&timeout: 타임아웃 값을 전달하는 부분
    - sizeof(timeout)
    */

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9999);

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);

    adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);

    char buf[1024];
    printf("[서버] 클라이언트 접속. 데이터를 기다림(5초 타임아웃)...\n");

    // --- 3. 클라이언트가 아무것도 안 보내면 여기서 5초 후 탈출 --- 
    int str_len = read(clnt_sock, buf, sizeof(buf));


    // EAGAIN: Resource temporarily unavailable
    // EWOULDBLOCK: 요청한 작업을 즉시 완료할 수 없으니, 나중에 다시 시도하라
    // 둘다 11로 정의되어 있다.
    if(str_len < 0){
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("[에러] 타임아웃 발생! 상대방이 너무 오랫동안 침묵하고 있습니다.\n");
        } else {
            perror("read 에러");
        }
    } else {
        printf("[서버] 받은 데이터: %s\n", buf);
    }
    close(clnt_sock);
    close(serv_sock);

    return 0;
}