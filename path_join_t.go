package main

import (
	"fmt"
	"path"
)

func main() {
	s := path.Join("/a", "bc", "def", "ghij", "klmno")
	fmt.Println(s)
}
