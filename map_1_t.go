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
		m2 = make(map[string]string) // map使用前必须先make
	} else {
		fmt.Println("world")
	}
	m2["toyota"] = "honda"
	s3 := "kaka"
	m["qiuxing"] = s3
	s3 = "AC kaka"
	fmt.Println(m)
	fmt.Println(s3)

	v1 := m["xxxxxxx"]
	fmt.Println("v1 is ", v1)

	mi := make(map[int]int)
	fmt.Println("mi v is ", mi[5])
}
