package main

import (
	"fmt"
	"time"
)

func main() {
	a := 3
	for a != 5 {
		fmt.Println("in loop")
		time.Sleep(2 * time.Second)
	}

}
