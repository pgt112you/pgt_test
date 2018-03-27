package main

import (
	"fmt"
	"strconv"
)

func main() {
	//var s string
	//s = ""
	//i, err := strconv.Atoi(s)
	//if err != nil {
	//	fmt.Println("111 convert err", err) // 111 convert err strconv.ParseInt: parsing "": invalid syntax
	//	return
	//}
	//fmt.Println("i is ", i)
	//ii, err := strconv.ParseUint(s, 10, 8)
	//if err != nil {
	//	fmt.Println("222 convert err", err)
	//	return
	//}
	//fmt.Println("ii is ", ii)

	us := strconv.FormatUint(64, 10)
	fmt.Println("us is ", us)
	return
}
