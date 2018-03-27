package main

import (
	"fmt"
)

const (
	A = 5
	B
	C = iota
	D = iota
	E
	F
)

func main() {
	fmt.Println(A)
	fmt.Println(B)
	fmt.Println(C)
	fmt.Println(D)
	fmt.Println(E)
	fmt.Println(F)
}
