package main

import (
	"fmt"
	"time"
)

func main() {
	bs := time.Now().Format("2006-01-02 15:04:05")
	bs1 := time.Now().Format("2007-01-02 16:24:05")
	es := time.Now().Format("2006-01-02 00:00:00")
	d, _ := time.ParseDuration("24h")
	es1 := time.Now().Add(d)
	es1s := es1.Format("2006-01-02 00:00:00")
	fmt.Println(bs)
	fmt.Println(bs1)
	fmt.Println(es)
	fmt.Println(es1s)
	es2 := time.Now().Format("2006-01-02")
	fmt.Println(es2)
	baseT, _ := time.Parse("20060102150405", "20170701000000")
	baseTime := baseT.UnixNano()
	fmt.Println(baseTime)

}
