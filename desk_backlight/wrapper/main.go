package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

type Data struct {
	Channel_1 int `json:"channel_1"`
	Channel_2 int `json:"channel_2"`
}

func main() {
	for {
		resp, err := http.Get("http://192.168.1.111")
		if err != nil {
			fmt.Println("No response from request")
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body) // response body is []byte

		var data Data
		if err := json.Unmarshal(body, &data); err != nil {
			fmt.Println(err)
		}

		fmt.Println(data)

		time.Sleep(1 * time.Second)
	}

}
