package main

import (
	"fmt"
	"unsafe"
)

func main() {
	var ui8 uint8 = 255
	var ui16 uint16 = 60000
	var ui32 uint32 = 64
	var ui64 uint64 = 64

	fmt.Printf("ui8 len is %d, value is %d\n", unsafe.Sizeof(ui8), ui8)
	fmt.Printf("ui16 len is %d, value is %d\n", unsafe.Sizeof(ui16), ui16)
	fmt.Printf("ui32 len is %d, value is %d\n", unsafe.Sizeof(ui32), ui32)
	fmt.Printf("ui64 len is %d, value is %d\n", unsafe.Sizeof(ui64), ui64)

	fmt.Println("==================================")

	var i8 int8 = 127     // 如果这个是255就编译不过，必须小于128才行
	var i16 int16 = 23767 // 这个如果是60000就编译必过，必须小于32768才行
	var i32 int32 = 64
	var i64 int64 = 64

	fmt.Printf("i8 len is %d, value is %d\n", unsafe.Sizeof(i8), i8)
	fmt.Printf("i16 len is %d, value is %d\n", unsafe.Sizeof(i16), i16)
	fmt.Printf("i32 len is %d, value is %d\n", unsafe.Sizeof(i32), i32)
	fmt.Printf("i64 len is %d, value is %d\n", unsafe.Sizeof(i64), i64)
}
