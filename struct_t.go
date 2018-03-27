package main

import (
	"fmt"
)

type tt struct {
	i int
	s string
}

func (this *tt) tf() {
	fmt.Println(this.i)
}

func (this tt) tf1() {
	fmt.Println(this.i)
}

func main() {
	var a, b tt
	a.i = 5
	a.s = "a"
	b.i = 5
	b.s = "a"
	if a == b {
		fmt.Println("a == b")
	} else {
		fmt.Println("a != b")
	}
	a.tf()
	(&a).tf1()

}
