package main

import (
    "time"
    "net"
    "fmt"
    "reflect"
)


var reportConn = NewReportConn()

type ReportConn struct {
    conn    net.Conn
    sending chan *int
}


func NewReportConn() *ReportConn {
    agentAddr, err := net.ResolveUDPAddr("udp", "127.0.0.1:3344")
    udpConn, err := net.DialUDP("udp", nil, agentAddr)

    if err != nil {
        fmt.Println("report dialudp agent error ", err)
        return nil
    }

    var c = &ReportConn{
        conn:    udpConn,
        sending: make(chan *int, 1024)}

    go c.send()

    return c
}

func (c *ReportConn) send() {
    fmt.Println("hello wrold!!!\n")
}


func main() {
    fmt.Println("main goroutine")
    time.Sleep(time.Second*16)
    fmt.Println("11 main goroutine")
    c := 0x01
    fmt.Printf("%T\n", c)
    var T1 = reflect.TypeOf(c)
    fmt.Println(T1)


}
