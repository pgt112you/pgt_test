package main

import (
	"errors"
	"fmt"
)

func main() {
	err1 := errors.New("hello world")
	err2 := fmt.Errorf("jia jie shi")
	s2 := fmt.Sprintf("%s", err2)
	fmt.Println(err1)
	fmt.Println(s2)
	fmt.Printf("%s\n", err1)
}
