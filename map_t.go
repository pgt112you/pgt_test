package main

import (
	"fmt"
	"runtime"
	"strings"
)

type TT struct {
	i int
	j int
}

func main() {
	var tt TT = TT{1, 2}
	m := make(map[int]TT)
	m[1] = tt
	tt.i = 11
	fmt.Println(tt)
	fmt.Println(m)
	fmt.Println("========================================================")
	m1 := make(map[int]*TT)
	pt := new(TT)
	pt.i = 1
	pt.j = 2
	m1[1] = pt
	pt1 := m1[1]
	delete(m1, 1)
	runtime.GC()
	fmt.Println(pt1)
	fmt.Println(m1)
	fmt.Println(pt)
	fmt.Println("--------------------------------------------------------")
	l := strings.Split("", ",")
	fmt.Println(l)
	fmt.Println(len(l))
	ll := []int{}
	fmt.Println(ll)
	fmt.Println(len(ll))
	fmt.Println("==========================================================")
	m11 := make(map[int]int)
	m11[5]++
	m11[5]++
	m11[6]++
	fmt.Printf("%v\n", m11)
	for k, v := range m11 {
		m11[k] = v + 2
	}
	fmt.Printf("%v\n", m11)
	fmt.Printf("%d\n", len(m11))

}
