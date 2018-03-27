package main

import (
	"fmt"
	"time"
)

func main() {

	var syncChan = make(chan int)

	var tick = time.NewTicker(time.Second)

	for {
		select {
		case _, ok := <-syncChan:
			if !ok {
				fmt.Println("syncChan is not ok")
				return
			}

		case <-tick.C:
			if time.Now().Unix()%10 == 0 {
				fc.clientconfig.SetLastSeq(fc.lastSeq)
			}
			if fc.IsClosed() {
				return
			}

			fc.NewLogComming()
		}
	}

}
