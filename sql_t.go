package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

func main() {
	dburl := fmt.Sprintf("%s:%s@tcp(%s:%s)/%s", "guangtong", "123qweQAZ", "121.42.48.131", "13307", "dbinstall")
	fmt.Println(dburl)
	db, err := sql.Open("mysql", dburl)
	if err != nil {
		fmt.Println("connect metric db err", err)
		return
	}
	defer db.Close()

	sqlstr := "SELECT installlog, monitorlog FROM install_log WHERE project=? and ip=? and port=? order by id desc limit 1"
	fmt.Println(sqlstr)
	rows, err := db.Query(sqlstr, "UOC", "127.0.0.1", "13307")
	if err != nil {
		fmt.Println("111111", err)
		return
	}
	defer rows.Close()

	for rows.Next() {
		var installLog, monitorLog string
		err = rows.Scan(&installLog, &monitorLog)
		if err != nil {
			fmt.Println(">>>>", err)
			break
		}
	}
	if err := rows.Err(); err != nil {
		fmt.Println("------", err)
	}
}
