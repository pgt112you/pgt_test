package main

import (
	"fmt"
	"time"
)

type cobj struct {
	i int
	s string
}

func funt(ch chan cobj) {
	select {
	case obj, ok := <-ch:
		fmt.Printf("%+v\n", ok)
		fmt.Printf("%+v\n", obj)
	}
}

func main() {
	cchan := make(chan cobj)
	go funt(cchan)
	time.Sleep(time.Second * 3)
	var obj cobj
	cchan <- obj
	time.Sleep(time.Second * 3)
}
