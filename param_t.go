package main

import (
	"fmt"
)

func tt(a, b int) {
	a = b
}

func main() {
	a := 5
	b := 6
	fmt.Println(a, b)

}
