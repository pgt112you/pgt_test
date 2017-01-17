package main

import (
	"fmt"
	"io"
	"net"
	"time"
)

func main() {
	conn, _ := net.Dial("tcp", "127.0.0.1:9112")
	io.WriteString(conn, "earth dream")
	var buf = []byte{}
	io.ReadFull(conn, buf)
	fmt.Println(buf)
	time.Sleep(time.Second * 60)
}
