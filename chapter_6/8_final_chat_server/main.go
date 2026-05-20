package main 

import (
	"fmt"
	"net"
	"bufio"
)

// 클라이언트 정보를 담는 구조체
type client chan<- string  // 메시지를 보낼 수 있는 전용 채널

var (
	entering = make(chan client)
	leaving = make(chan client)
	messages = make(chan string) // 모든 클라이언트가 보내는 메시지 공용 통로
)


// 1. Broadcaster: 모든 채팅 상태를 관리하는 '단일 소유자' 고루틴
func broadcast(){
	clients := make(map[client]bool) // 모든 접속자 명단(오직 이 고루틴만 접근함)
	for {
		select {
		case msg := <- messages:
			// 모든 접속자에게 메시지 전송
			for cli := range clients{
				cli <- msg
			}
		case cli := <-entering:
			clients[cli] = true 
		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

// 2. HandleConn: 각 클라이언트와 대화하는 개별 일꾼 고루틴
func handleConn(conn net.Conn){
	ch := make(chan string)
	go clientWriter(conn, ch) 

	who := conn.RemoteAddr().String()
	ch <- "당신의 주소는 " + who 
	messages <- who + " 님이 입장하셨습니다."
	entering <- ch 

	input := bufio.NewScanner(conn)
	for input.Scan(){
		messages <- who + ":" + input.Text()

	}
	
	leaving <- ch 
	messages <- who + "님이 퇴장하셨습니다."
	conn.Close()
}

func clientWriter(conn net.Conn, ch <- chan string){
	for msg := range ch {
		fmt.Fprintln(conn, msg)
	}
}

func main(){
	fmt.Println("[서버] 고성능 채팅 서버 시작 (Port 9999)....")
	lisenter, _ := net.Listen("tcp", "localhost:9999")

	go broadcast() // 관리자 고루틴 실행

	for {
		conn, _ := lisenter.Accept()
		go handleConn(conn) // 접속마다 일꾼 고루틴 실행
	}
}