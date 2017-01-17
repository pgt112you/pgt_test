package main

import (
	"fmt"
	"hash/crc32"
)

func main() {
	tb := crc32.MakeTable(crc32.IEEE)
	var value []byte = []byte("hello world aaa")
	var s uint32 = crc32.Checksum(value, tb)
	fmt.Println(s)
	var value1 []byte = []byte("earth dream 111111")
	var s1 uint32 = crc32.Update(s, tb, value1)
	fmt.Println(s1)
}
