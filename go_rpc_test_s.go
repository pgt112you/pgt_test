package main

import (
    "./t_arg"

    "fmt"
    "net"
	"net/rpc"
)


type TT_stru struct {
    i int
    b8 uint8
    s string
}



func (this *TT_stru) Deal(args *t_arg.Arg1, reply *t_arg.Arg2) error {
    this.i = this.i + args.Ii
    this.s = this.s + args.Ss
    fmt.Println("now i is", this.i)
    fmt.Println("now s is", this.s)
    return nil
}


func main() {
    tt := new(TT_stru)
    tt.i = 512
    tt.b8 = 128
    tt.s = "hello "
    rpc.Register(tt)
    l, _ := net.Listen("tcp", ":11234")
    rpc.Accept(l)

}

