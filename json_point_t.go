package main

import (
	"encoding/json"
	"fmt"
)

type A struct {
	Host string `json:"host"`
	Port int    `json:"port"`
}

type B struct {
	Master []A    `json:"master"`
	Slave  []A    `json:"slave"`
	M1     []*A   `json:"m1"`
	Num    int    `json:"num"`
	DB     string `json:"db"`
}

func main() {
	var a1, a11, a2, a22 A
	a1.Host = "10.138.227.187"
	a1.Port = 3306
	a11.Host = "10.138.227.187"
	a11.Port = 3307
	a2.Host = "10.138.227.186"
	a2.Port = 3306
	a22.Host = "10.138.227.186"
	a22.Port = 3307

	var a3, a33 A
	a3.Host = "10.138.227.185"
	a3.Port = 3306
	a33.Host = "10.138.227.185"
	a33.Port = 3307

	var b B
	b.Master = append(b.Master, a1)
	b.Master = append(b.Master, a11)
	b.Slave = append(b.Slave, a2)
	b.Slave = append(b.Slave, a22)
	b.Num = 35
	b.DB = "abc"
	b.M1 = append(b.M1, &a3)
	b.M1 = append(b.M1, &a33)
	v, _ := json.Marshal(b)
	fmt.Println(string(v))

	var b1 B
	err := json.Unmarshal(v, &b1)
	if err != nil {
		fmt.Println("err is ", err)
	}
	fmt.Println(b1.M1[0].Host)
}
