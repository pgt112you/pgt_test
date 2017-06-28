package main

import (
	"fmt"
	"strings"
)

func f1(f func(int, string) int, ss string) int {
	var i = 5
	res := f(i, ss)
	return res
}

func f(i int, s string) int {
	if strings.Compare("hello", s) == 0 {
		i += 5
	} else if strings.Compare("world", s) == 0 {
		i += 6
	} else {
		i += 9
	}
	return i
}

func main() {
	var s = "world"
	res := f1(f, s)
	fmt.Println(res)

}
