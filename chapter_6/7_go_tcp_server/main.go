package main 
import (
	"fmt"
	"bufio"
	"net"
)

func handleConnection(conn net.Conn){
	defer conn.Close()
	fmt.Printf("[서버] 클라이언트 접속: %s\n", conn.RemoteAddr().String())

	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		msg := scanner.Text()
		fmt.Printf("[수신] %s\n", msg)
		
		// 에코(받은 메시지 그대로 돌려주기)
		conn.Write([]byte("Echo: "+ msg + "\n"))
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("읽기 에러:", err)
	}
	fmt.Println("클라이언트 연결 종료")
}

func main(){
	// 1. listen(socket + bind + listen을 한번에 처리)
	ln, err := net.Listen("tcp", ":9999")

	if err != nil{
		fmt.Println("에러:", err)
		return 
	}
	
	defer ln.Close()
	fmt.Println("[서버] Go 에코 서버 시작(Port9999)...")
	for {
		// 2. Accept(새로운 접속 대기)
		conn, err := ln.Accept()

		if err != nil{
			continue
		}

		// 3. 고루틴 생성(접속마다 독립된 일꾼 배정)
		// c언어의 fork()나 pthread_create()보다 압도적으로 가볍고 빠름
		go handleConnection(conn)
	}
}