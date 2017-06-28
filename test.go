package main

import "fmt"
import "time"

func hello(i int) (j int) {
	j = i
	go helloworld()
	return j
}

func world(i int) int {
	i += 2
	go helloworld()
	return i
}

func helloworld() {
	time.Sleep(2000 * time.Millisecond)
	fmt.Println("HELLO WORLD")
}

func main() {
	fmt.Printf("hello world\n")
	a := 3
	var b int
	b = 5
	fmt.Printf("%d, %d\n", a, b)
	i := 5
	i = hello(6)
	j := world(8)
	fmt.Printf("%d, %d\n", i, j)
	var l []int
	fmt.Println(cap(l))
	fmt.Println(len(l))
	//append(l, 8)
	//fmt.Println(cap(l))
	//fmt.Println(len(l))
	time.Sleep(22000 * time.Millisecond)
	return
}
