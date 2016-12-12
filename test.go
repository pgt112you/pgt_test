package main

import "fmt"


func hello(i int) (j int) {
    j = i
    return j
}

func world(i int) (int) {
    i += 2
    return i
}

func main() {
    fmt.Printf("hello world\n")
    a := 3
    var b int
    b = 5
    fmt.Printf("%d, %d\n", a, b)
    i := 5
    i = hello(6)
    j := world(8)
    fmt.Printf("%d, %d\n", i, j)
    var l [3]int{2,3,4,5}
    fmt.Println(cap(l))
    fmt.Println(len(l))
    append(l, 8)
    fmt.Println(cap(l))
    fmt.Println(len(l))
}
