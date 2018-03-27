package main

import (
	"fmt"
	"os"
)

func main() {
	err := os.MkdirAll("t_dir", os.ModeDir|os.ModePerm)
	if err != nil {
		fmt.Println(err)
	}

}
