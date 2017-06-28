package main

import (
	"fmt"
	"time"
)

type AI int

func main() {
	fmt.Println(time.Now().Format("2006_01_02_15_04_05"))
	var i int = 5
	a := AI(i)
	fmt.Printf("i is %d, ai is %d\n", i, a)

}
