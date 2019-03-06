package main

import (
	"fmt"
	"strings"
)

func main() {
	ip := "192.168.2.3"
	nip := strings.Replace(ip, ".", "-", -1)
	fmt.Println("ip is ", ip)
	fmt.Println("nip is ", nip)

}
