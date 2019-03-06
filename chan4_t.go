package main

import (
	"time"
)

var c = make(chan int)
var a string

func f() {
	a = "hello, world"
	time.Sleep(time.Second * 2)
	<-c
}

func main() {
	go f()
	c <- 0
	print(a)
}
