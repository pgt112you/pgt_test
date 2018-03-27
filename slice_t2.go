package main

import (
	"fmt"
)

func tt(l []int) []int {
	l[3] = l[3] * 10
	return l
}

func main() {
	var la []int
	la = append(la, 1)
	la = append(la, 2)
	la = append(la, 3)
	la = append(la, 4)
	la = append(la, 5)
	lb := tt(la)
	fmt.Println(la)
	lb[2] = lb[2] * 10
	fmt.Println(la)
	fmt.Println(lb)
	fmt.Println(lb[2:50])
}
