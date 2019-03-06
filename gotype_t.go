package main

import (
	"fmt"
	"reflect"
)

func main() {
	m := make(map[string]string)
	m["aaa"] = "AAA"
	m["bbb"] = "BBB"
	v, ok := m["ccc"]
	fmt.Println("v is ", v)
	fmt.Println("ok is ", ok)
	fmt.Println("v type is ", reflect.TypeOf(v))
	m1 := make(map[string]map[string]int)
	temp := make(map[string]int)
	temp["hello"] = 1
	m1["world"] = temp
	v1, ok := m1["haha"]
	fmt.Println("v1 is ", v1)
	fmt.Println("v1 len is ", len(v1))
	fmt.Println("ok is ", ok)
	fmt.Println("v1 type is ", reflect.TypeOf(v1))
}
