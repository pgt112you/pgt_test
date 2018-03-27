package main

import (
	"fmt"
	"sort"
)

func main() {
	l := []int{1, 5, 3, 8, 5, 5, 2, 11, 59, -12, 29, 3}
	fmt.Println(l)
	sort.Ints(l)
	fmt.Println(l)
	for i := range l {
		fmt.Println(i)
	}
}
