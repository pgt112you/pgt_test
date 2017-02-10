package main

import (
    "./t_arg"

	"log"
	"net/rpc"
    "fmt"
)



func main() {
	client, err := rpc.Dial("tcp", "localhost:11234")
	if err != nil {
		log.Fatal(err)
	}

    var args t_arg.Arg1
    args.Ii = 3
    args.Ss = "world"
    var reply t_arg.Arg2
    err = client.Call("TT_stru.Deal", &args, &reply)
	if err != nil {
        fmt.Println("err: ", err) 
	}
    fmt.Printf("ii is %d, ss is %s\n", reply.Ii, reply.Ss)
}
