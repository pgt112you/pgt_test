package main

// #include "go_c_t.h"
// extern void show(char *);
import "C"
import "fmt"


func main() {
    var ss string = "Mazda"
    var ss1 *C.char;
    ss1 = C.CString(ss)
    fmt.Println(ss)
    fmt.Println(ss1)
    C.show(ss1)
}
