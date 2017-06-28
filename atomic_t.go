package main

import (
	"fmt"
	"sync/atomic"
)

func main() {
	var u uint32 = 5
	atomic.AddUint32(&u, 1)
	fmt.Println("u is ", u)
	u = atomic.AddUint32(&u, 1)
	fmt.Println("u is ", u)
}
