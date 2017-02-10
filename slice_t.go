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
    var ts TTS;
    ts.Encode(s)
    ts.i = 5
    ts.j = 6
    ts.k = 7
    ts.l = 8
    ts.m = 9
    fmt.Println(ts)

}
