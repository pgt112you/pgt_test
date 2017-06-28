package main

import (
	"fmt"
)

type SS struct {
	I int
	S string
}

func ff() []SS {
	return nil
}

func main() {
	v := ff()
	fmt.Println("hello")
	for _, s := range v {
		fmt.Printf("i is %d, s is %s\n", s.I, s.S)
	}
	fmt.Println("world")

}
