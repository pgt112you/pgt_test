package main

import (
	"fmt"
	"time"
)

func touch(ch *chan int) {
	var tick = time.NewTicker(time.Second * 3)
	i := 0
	for {
		select {
		case <-tick.C:
			i += 1
			*ch <- i
		}
	}
}

func main() {

	var syncChan = make(chan int)

	var tick = time.NewTicker(time.Second)

	for {
		select {
		case i, ok := <-syncChan:
			for i < 10 {
				if i == 5 {
					break
				}
			}

		case <-tick.C:
			fmt.Println()
		}
	}

}
