package main

import (
	"fmt"
	"time"
)

type Ss struct {
	s string
	i int
}

var ch chan int = make(chan int, 1)

func tryLock(waitMs int, word string) {
	select {
	case ch <- 1:
		fmt.Printf("%s get lock hello world\n", word)
	case <-time.After(time.Millisecond * time.Duration(waitMs)):
		fmt.Printf("%s get lock timeout\n", word)
	}

}

func main() {
	l := make([]Ss, 5)
	var s1 Ss
	s1.s = "hello world"
	s1.i = 39
	l = append(l, s1)
	fmt.Println(l)
	fmt.Println(s1)
	s1.s = "honda"
	fmt.Println(l)
	fmt.Println(s1)

	go func() {
		fmt.Println("in goroutine1\n")
		tryLock(5000, "goroutine1")
		fmt.Println("groutine1 over")
	}()
	go func() {
		fmt.Println("in goroutine2\n")
		tryLock(5000, "goroutine2")
		fmt.Println("groutine2 over")
	}()
	time.Sleep(time.Second * time.Duration(1))
	fmt.Println("main\n")
	tryLock(6000, "main")
	fmt.Printf("earth dream\n")
}
