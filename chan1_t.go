package main

import (
	"fmt"
	"time"
)

func printchan(ci chan int) {
	for {
		i := <-ci
		fmt.Println("read from ci ", i)
	}
}

func main() {
	var i int
	ci := make(chan int, 2)
	ci <- 6
	i = <-ci // 可以赋值chan里面的对象给i，但是不能把chan赋值给i
	fmt.Println("i is", i)
	fmt.Println("ci is", ci)

	// for range 是可以从channel里面读取数据的
	ci1 := make(chan int)
	go printchan(ci1)
	time.Sleep(time.Second)
	fmt.Println("hello world")
	for i = 0; i < 5; i++ {
		ci1 = append(ci1, i)
		fmt.Println("write in ci ", i)
		time.Sleep(time.Second)
	}
	close(ci1)
}
