package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {
	fmt.Println("hello world")
	time.Sleep(time.Second * 10)
	var ms runtime.MemStats
	runtime.ReadMemStats(&ms)
	fmt.Println(ms.NextGC)
	fmt.Println("===========")
}
