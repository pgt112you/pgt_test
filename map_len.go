package main

import "fmt"

func main() {
	m := make(map[string]int)
	m["hello"] = 3
	m["world"] = 4
	fmt.Println(len(m))
}
