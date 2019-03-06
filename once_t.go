package main

import "fmt"
import "sync"
import "time"

var a string

var i int
var once sync.Once

func setup() {
	i++
	a = fmt.Sprintf("hello, world %d\n", i)
}

func doprint() {
	once.Do(setup) // setup在整个程序中只运行一次，而不是在同一时间只能一个实力在运行
	print(a)
}

func twoprint() {
	go doprint()
	go doprint()
}

func main() {
	twoprint()
	time.Sleep(time.Second * 2)
}
