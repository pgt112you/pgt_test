package main

import (
	"fmt"
)

func f() []int {
	return nil
}

func main() {
	var l []int = f()
	fmt.Println(len(l))
}
