package main

import (
	"fmt"
)

func tt(pi *int) {
	*pi += 5
}

func main() {
	var i int = 5
	fmt.Println(i)
	tt(&i)
	fmt.Println(i) // 传指针是可以在函数里面改变这个值的
}
