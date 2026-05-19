package main

// runtime: Go 프로그램이 실행되는 동안 메모리 관리, 가비지 컬렉션, 고루틴 스케줄링 등 핵심 시스템 인프라와 상호작용할 수 있도록 돕는 표준 라이브러리
import (
	"fmt"
	"runtime"
	"time"
)

func main(){
	// 1. 실험 전 메모리 상태 측정
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	beforeAlloc := m.Alloc 

	fmt.Printf("[실험 전] 메모리 사용량: %.2f MB\n", float64(beforeAlloc)/1024/1024)

	numGoroutines := 100000 

	// 2. 고루틴 10만개 생성
	for i := 0; i < numGoroutines; i++{
		go func(){
			// 고루틴이 즉시 종료되지 않도록 대기
			time.Sleep(10 * time.Second)
		}()
	}

	// 고루틴 생성 완료 후 잠시 대기(스케줄링 시간 확보)
	time.Sleep(500 * time.Millisecond)

	// 3. 실험 후 메모리 상태 측정
	runtime.ReadMemStats(&m)
	afterAlloc := m.Alloc

	totalUsage := afterAlloc - beforeAlloc 
	perGoroutine := float64(totalUsage) / float64(numGoroutines)

	fmt.Println("\n --- 10만 개 고루틴 생성 완료 ---")
	fmt.Printf("[실험 후] 메모리 사용량: %.2f MB\n", float64(afterAlloc)/1024/1024)
	fmt.Printf("[결과] 증가한 총 메모리: %.2f MB\n", float64(totalUsage)/1024/1024)
	fmt.Printf("[결과] 고루틴 1개당 평균 점유량: 약 %.2f KB\n", float64(perGoroutine)/1024)

	fmt.Println("\n 다른 터미널에서 top이나 htop로 실제 프로세스 점유율 확인하기")
	time.Sleep(15 * time.Second) // 모니터링 시간 확보
}