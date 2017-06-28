package main

import (
	"fmt"
	"sync/atomic"
)

func main() {
	var i int32 = 5
	j := atomic.AddInt32(&i, -1)
	fmt.Println(i, j)

}
