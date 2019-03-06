package main

import (
	"fmt"
)

func tt(s []string) {
	s[2] = "iiiiiiiiiiiiiiiiiiiii"
}

func main() {
	s1 := make([]string, 5, 5)
	s1[0] = "0000"
	s1[1] = "1111"
	s1[2] = "2222"
	s1[3] = "3333"
	s1[4] = "4444"
	s2 := []string{"hello", "world", "jia", "jie", "shi"}

	fmt.Println(s1)
	fmt.Println(s2)
	tt(s1)
	tt(s2)
	fmt.Println("===============================")
	fmt.Println(s1)
	fmt.Println(s2)

}
