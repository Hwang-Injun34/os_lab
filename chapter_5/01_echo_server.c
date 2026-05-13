#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> // 소켓 함수

// 소켓 프로그래밍 시 IP 주소 변환(숫자 <-> 문자열)과 바이트 순서 변환(호스트 <-> 네트워)
#include <arpa/inet.h> 

/*
socket(): 전화기 만들기 | 새전화기 사기
bind(): 전화번호 설정 | 9999번 번호 부여
listen(): 대기 상태 | 전화 올 때 기다리기
accept(): 전화 받기 | 수화기 들기
read(): 손님 말 듣기 | "여보세요 뭐라고 했지?"
write(): 그대로 말하기 | "아~ 똑같이 따라하기" 
*/

/*
struct sockaddr_in {
    - sin_family(주소 체계)
    - sin_port(포트 번호)
    - sin_addr(IP 주소)
        - in_addr( -> s_addr(32비트 IP 주소 지정))
    - sin_zero(0으로 채워진 패딩)

}
*/

#define PORT 9999

int main(){
    // 서버 소켓(문지기), 클라이언트 전용 소켓(통신용)
    int serv_sock, clnt_sock;
    // 서버 주소 정보, 클라이언트 주소 정보
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char message[1024];
    int str_len;


    // --- 1. 소켓 생성 (전화기 구입) --- 
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    // PF_INET(Protocl Family for Internet(IPv4))
    // SOCK_STREAM: 소켓 타입이 스트림방식
    // 0: 기본 프로토콜인 TCP가 자동으로 선택
    

    // --- 2. 주소 설정(전화번호 할당) --- 
    memset(&serv_addr, 0, sizeof(serv_addr)); // 구조체 초기화
    // memset(설정하고자하는 메모리, 설정값, 값을 채울 바이트 크기)

    // 주소 체계
    serv_addr.sin_family = AF_INET; 
    // AF_INET(IPv4): Address Family for Internet, 주소 형식을 지정할 때 사용
    
    // IP 주소
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    // s_addr: in_addr 구조체의 멤버, IPv4 주소
    // htonl: host to network long, 호스트 바이트 순서로 저장된 32비트 숫자(주소)를 네트워크 바이트(빅 엔디안)으로 변환하는 함수
    // INADDR_ANY(0.0.0.0): 소켓이 컴퓨터의 모든 IP주소(NIC가 여러 개인 경우 포함)에서 들어오는 요청을 수신하도록 자동 할당
    // 즉, 이 서버는 모든 네트워크 인터페이스(IP)에서 들어오는 요청을 받는다.
    // 로컬에서만 할거면 "127.0.0.1"로 설정해야함

    // 포트 지정
    serv_addr.sin_port = htons(PORT);
    // htons: host to network short(2바이트 짧은 정수), 호스트 컴퓨터의 바이트 순서(보통 리틀 엔디안)를 네트워크 표준 바이트 순서(빅 엔디안)로 변환한다.

    // --- 3. 주소 할당 --- 
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // bind: 소켓(serv_sock)에 주소 정보를 묶는(할당) 함수
    // serv_sock: socket() 함수로 생성된 서버의 소켓 파일 디스크립터
    // (strcuct sockaddr*): bind함수는 범용 주소 구조체인 struct sockaddr * 타입을 인자로 받음
    // sizeof(serv_addr): 주소 구조체의 크기를 바이트 단위로 전달

    // --- 4. 대기 상태로 전환 --- 
    listen(serv_sock, 5);
    // listen: TCP/IP 소켓 프로그래밍에서 서버 소켓을 연결 요청 대기 상태로 전환하는 함수 
    // 최대 5개의 연결까지 대기 큐에 수용
    // serv_sock: 서버 소켓 파일 디스크립터, 이 소켓은 이제 클라이언트의 연결을 받는 문지기 역할 수행
    // 5(두번째 인자 - Backlog): 연결 요청 대기 큐, 이 서버가 동시에 처리할 수 있는 접속 대기 요청의 수, 최대 5개까지 큐에 쌓을 수 있음

    // ---  5. 연결 수락(전화 받기) -> 통신용 새로운 FD 생성됨 --- 
    clnt_addr_size = sizeof(clnt_addr);
    // 클라이언트의 주소 정보를 담을 구조체의 크기 계산

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    // accept: 서버 소켓(serv_sock)에 대기 중인 클라이언트의 연결 요청 큐에서 첫 번째 연결 요청을 꺼내어 연걸. 연결이 성공하면 클라이언트와 통신할 수 있는 새로운 소켓 파일 디스크립터를 반환
    // serv_sock: bind()와 linsten()을 마친 서버의 대기 소켓 
    // (struct sockaddr*)&clnt_addr: 연결된 클라이언트의 주소 정보가 저장될 구조체 변수의 포인터
    // &clnt_addr_size: clnt_addr 구조체의 크기를 담고 있는 변수의 포인터
    // clnt_sock: 반환값, 성공시에는 클라이언트와 1:1 통신을 위한 새로운 소켓 디스크립터, 실패시 -1

    printf("[서버] 클라이언트 연결 성공\n");

    // -- 6. 데이터 수신 및 그대로 돌려주기(Echo) --- 
    while((str_len = read(clnt_sock, message, sizeof(message))) != 0){
        write(clnt_sock, message, str_len);
        message[str_len] = '\0';
        printf("[서버] 받은 메시지: %s", message);
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}