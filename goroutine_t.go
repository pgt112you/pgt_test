package main

import (
	"fmt"
	"time"
)

type TT struct {
	I int
	s string
}

func NewTT(s string) *TT {
	t := new(TT)
	t.s = s
	go t.Run()
	return t
}

func (t *TT) Run() {
	for {
		fmt.Println(t.s)
		time.Sleep(time.Second * 2)
	}
}

func main() {
	var m map[int]*TT
	t1 := NewTT("hello")
	t2 := NewTT("world")

	m = make(map[int]*TT)
	m[1] = t1
	m[2] = t2

	time.Sleep(time.Second * 20)

	delete(m, 1)

	time.Sleep(time.Minute * 20)

}
