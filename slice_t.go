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
	//s2 = s1[2:50]  // 后面的下标不能越界
	fmt.Printf("%v\n", s2)
	//fmt.Printf("\n nil len is %d\n", len(nil))   // nil 不能用len
	fmt.Println("======================================================")
	var sl1 []int = []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}
	fmt.Printf("aaa %v\n", sl1[:0])
	fmt.Printf("bbb %v\n", sl1[9:])
	var sl2 []int
	sl2 = append(sl2, sl1[10:]...)
	fmt.Printf("ccc %v\n", sl2)
}
