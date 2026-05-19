package main 
import (
	"fmt"
	"time"
)

func main(){
	// 1. 버퍼가 3개인 채널 생성
	// 수신자가 없어도 3개까지 송신자 멈추지 않음
	logChan := make(chan string, 3)
	quitChan := make(chan bool)

	// 2. 로그 처리기(수신자)
	go func(){
		for {
			select {
			case log := <- logChan: 
				fmt.Printf("[로그 서버] 처리 중: %s\n", log)
				time.Sleep(1 * time.Second)
				
			case <- quitChan:
				fmt.Println("[로그 서버] 종료 요청 수신. 시스템을 닫습니다.")
				return 

			case <- time.After(5 * time.Second):
				fmt.Println("[경고] 5초간 활동이 없다. 대기 모드 진입")
			}
		}
	}()

	// 3. 로그 생성기(송신자)
	fmt.Println("[생성기] 로그 3개를 빠르게 던진다.")
	logChan <- "로그 #1"
	logChan <- "로그 #2"
	logChan <- "로그 #3"
	fmt.Println("[생성기] 버퍼가 있어서 멈추지 않고 즉시 다음 작업을 진행")

	time.Sleep(5 *time.Second)
	quitChan <- true 
	time.Sleep(1 * time.Second)
}