package main

import (
	"bufio"
	"database/sql"
	"github.com/go-sql-driver/mysql"
	"log"
	"os"
	"regexp"
	"strconv"

	//"strconv"

	//"bufio"
	"fmt"
	//"io/ioutil"
	yaml "gopkg.in/yaml.v2"
	"net/http"
	"time"
)

type Config struct {
	Mysql struct {
		Address  string `yaml:"address"`
		User     string `yaml:"user"`
		Password string `yaml:"password"`
		Db       string `yaml:"db"`
	} `yaml:"mysql"`
}

var cfg Config

type Record struct {
	Channel_34 int
	Channel_35 int
	Program    int
}

var doneChannel = make(chan bool)
var ch = make(chan Record)

func main() {
	f, err := os.Open("./conf.yml")

	if err != nil {
		//processError(err)
	}
	defer f.Close()

	decoder := yaml.NewDecoder(f)
	err = decoder.Decode(&cfg)
	if err != nil {
		//processError(err)
	}

	fmt.Println(cfg)

	db := connect()

	go saveData(db)

	for {
		resp, err := http.Get("http://192.168.1.111/metrics")
		if err != nil {
			fmt.Println("No response from request")
		}
		defer resp.Body.Close()

		scanner := bufio.NewScanner(resp.Body)
		scanner.Split(bufio.ScanLines)

		var tmp Record

		for scanner.Scan() {
			re, _ := regexp.Compile(`{channel="(.*)+"}(\d+)`)
			res := re.FindStringSubmatch(scanner.Text())

			switch res[1] {
			case "program":
				tmp.Program, _ = strconv.Atoi(res[2])
			case "34":
				tmp.Channel_34, _ = strconv.Atoi(res[2])
			case "35":
				tmp.Channel_35, _ = strconv.Atoi(res[2])
			}
		}

		ch <- tmp

		time.Sleep(10 * time.Second)

	}

	<-doneChannel
}

func connect() *sql.DB {
	mysqlCfg := mysql.Config{
		User:                 cfg.Mysql.User,
		Passwd:               cfg.Mysql.Password,
		Net:                  "tcp",
		Addr:                 cfg.Mysql.Address,
		DBName:               cfg.Mysql.Db,
		AllowNativePasswords: true,
	}

	fmt.Println(mysqlCfg.FormatDSN())

	db, err := sql.Open("mysql", mysqlCfg.FormatDSN())
	db.SetMaxOpenConns(10)
	db.SetMaxIdleConns(10)

	if err != nil {
		log.Fatal(err)
	}

	pingErr := db.Ping()

	if pingErr != nil {
		fmt.Println("error ")
		log.Fatal(pingErr)
	} else {
		fmt.Println("Connected!")
		return db
	}

	return nil
}

func saveData(db *sql.DB) {
	//db.Query("truncate test")

	stmt, err := db.Prepare("INSERT INTO `sensors_stat` (`channel_34`, `channel_35`, `program`) values (?,?,?)")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()

	for d := range ch {
		fmt.Println(d)
		_, err := stmt.Exec(d.Channel_34, d.Channel_35, d.Program)
		if err != nil {
			log.Fatal(err)
		}

		fmt.Println("write db", d)
	}

}
