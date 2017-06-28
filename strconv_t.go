package main

import (
	"fmt"
	"strconv"
)

func main() {
	var s string
	s = ""
	i, err := strconv.Atoi(s)
	if err != nil {
		fmt.Println("convert err", err)
		return
	}
	fmt.Println("i is ", i)
	return
}
