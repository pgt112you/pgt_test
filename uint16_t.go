package main

import (
	"fmt"
	"sync/atomic"
)

func main() {

	var i32 uint32 = 4294967294
	i32 = atomic.AddUint32(&i32, 1)
	fmt.Println("111 ", i32)
	i32 = atomic.AddUint32(&i32, 1) // 会溢出，从0开始
	fmt.Println("222 ", i32)
	i32 = atomic.AddUint32(&i32, 1)
	fmt.Println("222 ", i32)

}
