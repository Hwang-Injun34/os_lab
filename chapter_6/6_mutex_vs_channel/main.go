package main 

import (
	"fmt"
	"sync"
	"time"
)

// 1. Mutex 방식: 공유 메모리를 락으로 보호
func useMutex(iterations int){
	var mu sync.Mutex 
	count := 0 
	var wg sync.WaitGroup 

	start := time.Now()
	for i:= 0; i < iterations; i++{
		wg.Add(1)
		go func(){
			defer wg.Done()
			mu.Lock()
			count++
			mu.Unlock()
		}()
	}

	wg.Wait()
	fmt.Printf("[Mutex] 결과: %d, 소요시간: %v\n", count, time.Since(start))
}

// 2. Channel 방식: 데이터를 채널로 전달하여 처리
func useChannel(iterations int){
	count := 0 
	ch := make(chan int)
	var wg sync.WaitGroup 

	start := time.Now()

	go func(){
		for range ch {
			count++ 
			wg.Done()
		}
	}()

	for i := 0; i < iterations; i++{
		wg.Add(1)
		ch <- 1 
	}
	wg.Wait()
	close(ch)
	fmt.Printf("[Channel] Result: %d, 소요시간: %v\n", count, time.Since(start))
}

func main(){
	const iterations = 100000
	fmt.Println("--- Mutex vs Channel 성능 비교 ---")
	useMutex(iterations)
	useChannel(iterations)
	
}