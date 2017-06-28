package main

import (
	"fmt"
)

type TTS struct {
	i int
	j int
	k int
	l int
	m int
}

func (tts *TTS) Encode(pkg []int) {
	pkg[0] = tts.i
	pkg[1] = tts.j
	pkg[2] = tts.k
	pkg[3] = tts.l
	pkg[4] = tts.m
}

func main() {
	s := make([]int, 5)
	var ts TTS
	ts.Encode(s)
	ts.i = 5
	ts.j = 6
	ts.k = 7
	ts.l = 8
	ts.m = 9
	fmt.Println(ts)

	fmt.Println("-------------------------------------------------------------------")

	var s1 []int
	var s2 []int
	for i := 0; i < 9; i++ {
		s1 = append(s1, i)
	}
	for i := 10; i < 19; i++ {
		s2 = append(s2, i)
	}

	s1 = append(s1, s2...)
	fmt.Printf("%v", s1)
	fmt.Println()
	s2 = s1[2:]
	fmt.Printf("%v", s2)
	fmt.Printf("\n nil len is %d\n", len(nil))
}
