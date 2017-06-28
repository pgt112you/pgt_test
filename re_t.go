package main

import (
	"fmt"
	"regexp"
)

func main() {
	var s string = "/wtable/v1/c001/config/proxy/192.168.56.101:6698"
	var s1 string = "/wtable/v1/c001/config/db/db1"
	var prefix string = "/wtable/v1/c001/config"
	var rp []*regexp.Regexp = []*regexp.Regexp{
		regexp.MustCompile(prefix + `/slot/s(\d+)$`),
		regexp.MustCompile(prefix + `/db/db(\d+)$`),
		regexp.MustCompile(prefix + `/group/g(\d+)/(\w+)$`),
		regexp.MustCompile(prefix + `/punish/db(\d+)$`),
		regexp.MustCompile(prefix + `/proxy/(.+)$`)}

	m := rp[1].FindAllStringSubmatch(s1, -1)
	fmt.Println(m)
	m = rp[4].FindAllStringSubmatch(s, -1)
	fmt.Println(m)
	fmt.Println("-------------------------------------------------------")
	re := regexp.MustCompile(`/wtable/v1/server/(.+)/(\d+)$`)
	ss := "/wtable/v1/server/10.136.224.136/39890"
	res := re.FindStringSubmatch(ss)
	fmt.Printf("===%q", res)

}
