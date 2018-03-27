package main

import (
	"fmt"
)

func main() {
	var l []int
	l = append(l, 1)
	l = append(l, 2)
	l = append(l, 3)
	l = append(l, 4)
	l = append(l, 5)
	//a := append(l[:0], l[1:]...)
	//fmt.Println(a)
	fmt.Println(append(l[:4], l[5:]...))

}
