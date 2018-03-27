package main

import (
	"fmt"
)

func main() {
	s := "hello_world_green_earth"
	var a, b, c, d string
	var i int
	//n, err := fmt.Sscanf(s, "%d_%s_%s_%s_%s", i, a, b, c, d)
	n, err := fmt.Sscanf(s, "%s_%s_%s_%s", &a, &b, &c, &d)
	fmt.Printf("%v\t%v\n", n, err)
	fmt.Println("=========================")
	fmt.Println(i)
	fmt.Println(a)
	fmt.Println(b)
	fmt.Println(c)
	fmt.Println(d)
}
