package main  

import (
    "fmt"
    "time"
)

type client chan <- string 

func main(){
    ch := make(chan string)

    var c client = ch 

    go func(){
        for msg := range ch {
            fmt.Printf("[클라이언트 수신 화면] %s\n", msg)
        }
    }()

    fmt.Println("[서버] 클라이언트에게 메시지를 보낸다.")
    c <- "안녕하세요"
    c <- "고 언어 학습 중"

    time.Sleep(100 * time.Millisecond)
    fmt.Println("[서버] 프로그램 종료")

}