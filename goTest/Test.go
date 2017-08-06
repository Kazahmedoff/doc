package main

import (
	"fmt"
	"bufio"
	"os"
	"io/ioutil"
	"strings"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	path := scanner.Text()
	fmt.Println(getStringCount(path))
}

func getStringCount(path string) int {
	content, error := ioutil.ReadFile(path)

	if error != nil {
		fmt.Println("This file is not exist!")
		os.Exit(1)
	}

	if  len(content) == 0 {
		return 0;
	}

	text := string(content)
	count := strings.Count(text, "\n") + 1
	return  count
}
