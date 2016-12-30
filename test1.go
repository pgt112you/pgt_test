package main

import (
    "fmt"
    "time"
)


func main() {
    t := time.Now()
    fmt.Printf("%d%d", t.Year()%10, (t.Month()-1)/3)
}
