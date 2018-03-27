package main

import (
	"fmt"
	"time"
)

func show() {
	fmt.Println("hello world")
}

func main() {
	for {
		defer show() // defer只能在退出函数时调用，不能再推出代码块的时候调用，所以这里是调用不了的
		fmt.Println("jiajieshi")
		time.Sleep(time.Second)

	}

}
