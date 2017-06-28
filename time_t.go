package main

import (
	"fmt"
	"time"
)

func main() {
	bs := time.Now().Format("2006-01-02 15:04:05")
	es := time.Now().Format("2006-01-02 00:00:00")
	d, _ := time.ParseDuration("24h")
	es1 := time.Now().Add(d)
	es1s := es1.Format("2006-01-02 00:00:00")
	fmt.Println(bs)
	fmt.Println(es)
	fmt.Println(es1s)
	es2 := time.Now().Format("2006-01-02")
	fmt.Println(es2)

}
