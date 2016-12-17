package main

import (
    "fmt"
    "time"
)

func main() {
    fmt.Println("When's Wednesday?")
    today := time.Now().Weekday()
    switch time.Wednesday {
    case today + 2:
        fmt.Println("In two days.")
        fallthrough   // fallthrough的吓一跳必然执行，不管case是否匹配的上
    case today + 0:
        fmt.Println("Today.")
    case today + 1:
        fmt.Println("Tomorrow.")
    default:
        fmt.Println("Too far away.")
    }


    t := time.Now()
    switch {
    case t.Hour() < 12:
        fmt.Println("Good morning!")
    case t.Hour() < 17:
        fmt.Println("Good afternoon.")
    default:
        fmt.Println("Good evening.")
    }
    var ll [3]int = [3]int{1,2,3}
    fmt.Println(len(ll))
    fmt.Println(cap(ll))
    var ss string
    ss = fmt.Sprintf("hello world month %02d\n", time.Now().Month())
    fmt.Println(ss)
}
