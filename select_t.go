package main

import (
	"fmt"
)

func main() {
	var ci = make(chan int, 1) // 这样case ci <- 5是会走进去的
	//var ci chan int
	ci = nil
	var ci1 = make(chan int, 1)

	select {
	case ci <- 5:
		fmt.Println("in ci ")
	case <-ci1:
		fmt.Println("in ci1 ")
	default:
		fmt.Println("in default")
	}

}
