package main

import (
	"fmt"
)

func main() {

	var ss []string = []string{"changan", "changcheng", "guangqi", "jili", "shangqi"}
	for s := range ss {
	LOOP:
		for j := 0; j < 5; j++ {
			for i := 0; i < 5; i++ {
				fmt.Println(i)
				if i == 2 {
					break LOOP
				}
			}
			fmt.Println("in string for after one for s is ", s)
		}

		fmt.Println("in string for after tow for s is ", s)
	}
	fmt.Println("after string for ")

}
