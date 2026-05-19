package main  

import (
	"fmt"
	"sync"
)

func main(){
	var count = 0 
	var wg sync.WaitGroup 

	// 2개의 고루틴이 하나의 변수 count를 마구 수정함(Lock 없음)
	for i := 0; i < 1000; i++{
		wg.Add(1)
		go func(){
			defer wg.Done()
			count++; // 데이터 경쟁 발생 지점
		}()
	}

	wg.Wait()
	fmt.Printf("최종 결과값: %d\n", count)

}