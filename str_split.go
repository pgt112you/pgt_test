package main

import "fmt"
import "strings"

func main() {
	var s string = ""
	ss := strings.Split(s, ",")
	fmt.Println(ss)
	fmt.Println(len(s))
}
