package main

import (
	"fmt"
	"net"
)

func main() {
	ns, err := net.LookupHost("www.baidu.com")
	if err != nil {
		fmt.Printf("Err: %s", err.Error())
		return
	}

	for _, n := range ns {
		fmt.Printf("--%s\n", n)
	}

}
