#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    int str_len;

    // --- 1. 소켓 생성 --- 
    sock = socket(PF_INET, SOCK_STREAM, 0); 

    // --- 2. 서버 주소 설정 ---  
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serv_addr.sin_port = htons(9999);

    // --- 3. 서버에 연결 시도 --- 
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        // connect: 클라이언트가 서버에 연결을 요청할 때 사용하는 POSIX 표준 소켓 함수 
        perror("connect() 에러");
        exit(1);
    }

    printf("[클라이언트] 서버에 연결\n");

    while(1) {
        fputs("메시지 입력(Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;

        write(sock, message, strlen(message));
        str_len = read(sock, message, sizeof(message) - 1);
        message[str_len] = '\0';
        printf("[클라이언트] 서버로부터 Echo: %s", message);
    }

    close(sock);
    return 0;
}