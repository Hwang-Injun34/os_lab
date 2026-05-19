package main 

import (
	"fmt"
	"runtime"
)

func main(){
	var m runtime.MemStats
	runtime.ReadMemStats(&m)

	fmt.Printf("사용 중인 스택 메모리: %d 바이트\n", m.StackInuse)
	fmt.Printf("실행 중인 고루틴 개수: %d 개\n", runtime.NumGoroutine())

}

// StackInuse: 현재 활성화된 고루틴들이 스택 메모리 스팬에서 실제로 사용중인 메모리의 양을 나타내는 런타임