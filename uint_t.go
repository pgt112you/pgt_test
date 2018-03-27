package main

import (
	"fmt"
)

func main() {
	var a uint = 12
	var b uint = 14
	c := a - b
	fmt.Printf("%T,%[1]t %[1]d\n", c)
	var d uint8
	var i int = -1
	tempb := byte(i)
	d = uint8(tempb)
	fmt.Println(d)

}
