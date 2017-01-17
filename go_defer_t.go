package main

import (
	"fmt"
)

type Hello struct {
	i int
}

func (h *Hello) show() {
	fmt.Printf("in defer hello i is %d\n", h.i)
}

func (h Hello) show11() {
	fmt.Printf("hello i is %d\n", h.i)
}

func main() {
	defer func() { fmt.Println("in main defer") }()
	var i int = 3
	if i > 0 {
		defer func() { fmt.Println("in if defer") }()
		fmt.Println("hello")
	} else {
		fmt.Println("world")
	}

	fmt.Println("===================================")

	var l [3]int = [3]int{1, 2, 3}
	for _, v := range l {
		var h *Hello = new(Hello)
		h.i = v
		h.show11()
		fmt.Printf("i is %d\n", v)
		defer h.show()
		// 下面注释掉的和上面的都可以，都可以。。。。
		/*
			var h Hello
			h.i = v
			h.show11()
			fmt.Printf("i is %d\n", v)
			var ph *Hello = &h
			defer ph.show()
		*/
	}
}
