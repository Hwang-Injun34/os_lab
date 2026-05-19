package main 

import (
	"fmt"
	"time"
)

func main(){
	// 1. 버퍼가 없는 채널 생성(Unbuffered Channel)
	ch := make(chan string)

	// 2. 수신자 고루틴(느린 일꾼)
	go func(){
		fmt.Println("[수신자] 3초 뒤에 데이터를 받을 준비가 된다...")
		time.Sleep(3 * time.Second)

		msg := <- ch 
		fmt.Printf("[수신자] 데이터 수신 성공:%s\n", msg)
	}()

	// 3. 송신자(메인 고루틴)
	fmt.Println("[송신자] 데이터를 채널에 던진다...")

	// 버퍼가 없으므로 수신자가 <-ch를 호출할 때까지 여기서 멈춤(Blocking)
	ch <- "비밀 메시지"

	fmt.Println("[송신자] 수신자가 받아갔으므로 이제 다음 일을 한다.")

	// 결과 확인을 위해 잠시 대기
	time.Sleep(1 * time.Second)
}