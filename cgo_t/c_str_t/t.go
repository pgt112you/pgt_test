package main

// #include "ct.h"
import "C"

import (
	"fmt"
	"unsafe"
)

func main() {
	var i C.int = 5
	var c1 *C.char = C.print(i)
	fmt.Println("c1", C.GoString(c1))
	C.free(unsafe.Pointer(c1))
	var j C.int = -5
	var c2 *C.char = C.print(j)
	if c2 == nil {
		fmt.Println("c2 is NULL")
	} else {
		fmt.Println("c2", C.GoString(c2))
	}
}
