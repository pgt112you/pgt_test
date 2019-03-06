package main

import (
	"fmt"
	"reflect"
)

type A struct {
	i int
	s string
}

func main() {
	m := make(map[string]A)
	a1 := A{1, "a"}
	a2 := A{2, "b"}
	a3 := A{3, "c"}
	m["hello"] = a1
	m["world"] = a2
	m["test"] = a3
	if vobj, ok := m["world"]; ok {
		fmt.Println(">>>>", reflect.TypeOf(vobj))
		vobj.i += 10
	}
	fmt.Println(m)
}
