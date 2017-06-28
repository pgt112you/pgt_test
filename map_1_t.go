package main

import (
	"fmt"
)

func main() {
	m := make(map[string]string)
	m["hello"] = "world"
	m["jiajieshi"] = "gaolujie"

	var m2 map[string]string
	fmt.Println(m)
	fmt.Println(len(m))
	fmt.Println(len(m2))
	if m2 == nil {
		fmt.Println("hello")
	} else {
		fmt.Println("world")
	}
	m2["toyota"] = "honda"
}
