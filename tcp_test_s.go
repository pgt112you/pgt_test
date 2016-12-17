package main

import (
    "fmt"
    "net"
)


func main() {
    ln, err := net.Listen("tcp", ":8080")
    if err != nil {
        fmt.Printf("listen 8080 error %s\n", err)
    }

    for {
    	conn, err := ln.Accept()
    	if err != nil {
    		continue
    	}
    	go handleConnection(conn)
    }


}
