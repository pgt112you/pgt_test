package main

import "fmt"

func main() {
	var i uint8 = 0
	var ii int32
	for ii = 0; ii < 1000; ii++ {
		i += 1
		fmt.Println(i)
	}

}
