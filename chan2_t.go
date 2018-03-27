package main

import (
	"fmt"
	"time"
)

type Ready struct {
	i int
}

func printchan(ci chan Ready) {
	for {
		x := <-ci
		fmt.Println("read from ci ", x.i)
	}
}

func main() {
	var re1 Ready
	re1.i = 6
	var re2 Ready
	ci := make(chan Ready, 2)
	go printchan(ci)
	ci <- re1
	ci <- re2

	time.Sleep(time.Second * 6)
	close(ci)
}
