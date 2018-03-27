package main

import (
	"fmt"
	"time"
)

func formatTime(t time.Time) string {
	return t.Format("2006-01-02 15:04:00")
}

func main() {
	m := make(map[int]int)
	fmt.Println(m[5])
	fmt.Println(m[0])
	n := time.Now()
	t := n.Format("2006-01-02 15:04:00")
	fmt.Printf("%v\n", t)
	fmt.Printf("%v\n", n)
	n1 := time.Now()
	fmt.Printf("=== %v\n", n1)
	var l [4]int
	fmt.Println(l[2])

}
