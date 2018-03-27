package main

import (
	"fmt"
	"net/http"
)

func testfunc(w http.ResponseWriter, r *http.Request) {
	p1 := r.FormValue("p1")
	p2 := r.FormValue("p2")
	fmt.Println(p1, "====", p2)
	fmt.Printf("p1: %T, p2: %T\n", p1, p2)
}

func main() {
	http.HandleFunc("/test", testfunc)

	err := http.ListenAndServe("127.0.0.1:8080", nil)
	if err != nil {
		fmt.Printf("Listen failed: %s", err)
	}
}
