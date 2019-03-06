package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	url := "http://127.0.0.1:8080/mysql2/COSMOI"
	resp, err := http.Get(url)
	if err != nil {
		fmt.Println("err is ", err)
	}
	defer resp.Body.Close()
	fmt.Println(resp.Code)

}
