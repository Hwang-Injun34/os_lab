package main 

import (
	"fmt"
	"runtime"
	"time"
)

func main(){
	// 현재 PC의 논리 프로세서(코어) 개수 확인
	cpuCores := runtime.NumCPU()
	fmt.Printf("[환경] 시스템 논리 코어 수: %d\n", cpuCores)

	// 고루틴을 실행할 최대 OS 스레드 수 설정(기본값은 코어 수)
	// runtime.GOMAXPROCS(2), 필요시 강제로 제한 가능
	numGoroutines := 1000 // 1,000개의 고루틴 생성

	for i:= 0; i < numGoroutines; i++{
		go func(id int){
			for {
				// 무한 루프를 돌며 CPU를 소량 점유
				_ = id * id 
				time.Sleep(time.Millisecond)
			}
		}(i)
	}
	fmt.Printf("[수행] %d개의 고루틴이 실행 중입니다...\n", numGoroutines)
	fmt.Println("[명령] 다른 터미널에서 다음 명령어를 입력해 실제 스레드 수(LWP)를 확인하세요.")
	fmt.Println("명령어: ps -eLf | grep main | wc -l")
}