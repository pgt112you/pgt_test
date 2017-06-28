package main

//
//#cgo CFLAGS:-I ./
//#cgo LDFLAGS:-L ./
//#include <stdlib.h>
//#include "str_t.h"
//#include "num_t.h"
//
//int add1(int a, int b) {
//    return a+b;
//}
import "C"

import (
	"fmt"
	"unsafe"
)

func main() {
	var i C.int = 3
	var j C.int = 5
	res := C.add1(j, i)
	fmt.Println(res)
	fmt.Println("======================================================================")
	var s1 string = "hello world "
	s2 := "earth dream"
	cs1 := C.CString(s1)
	cs2 := C.CString(s2)
	var cs3 = C.concatenate1(cs1, cs2)
	fmt.Printf("%T\n", cs3)
	s3 := C.GoString(cs3)
	fmt.Println("----------------------------------------------------------------------")
	dcs2 := (unsafe.Pointer)(cs2)
	C.free(dcs2)
	fmt.Println("--------------------------------======================================")
	dcs1 := (unsafe.Pointer)(cs1)
	C.free(dcs1)
	fmt.Println("--------------------------------======================================")
	dcs3 := (unsafe.Pointer)(cs3)
	C.free(dcs3)
	fmt.Println("--------------------------------======================================")
	//var s4 string
	//C.concatenate2(s1, s2, s4)
	fmt.Println("s3", s3)
	fmt.Println("======================================--------------------------------")
	//fmt.Println("s4", s4)
	var a C.int = 9
	var b C.int = 4
	res1 := C.add(a, b)
	res2 := C.substra(a, b)
	fmt.Println(res1, res2)

}
