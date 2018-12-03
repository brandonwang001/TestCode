package main

import (
	"fmt"
	"time"
)

var (
	pi float32 = 3.1415926
)

func frac(a int) int {
	if a == 1 {
		return 1
	} else {
		return a * frac(a-1)
	}
}

func GetCounter() func() int {
	i := 0
	return func() int {
		i += 1
		return i
	}
}

func area(length float32, width float32) float32 {
	c := length * width
	return c
}

func CircleArea(r float32) float32 {
	return pi * r * r
}

func add(a int, b int) int {
	return a + b
}

func SumAndCompare(a int, b int, c int) (bool, int) {
	d := add(a, b)
	if d > c {
		return true, c
	} else {
		return false, c
	}
}

func swap1(a int, b int) {
	c := a
	a = b
	b = c
}

func swap2(a *int, b *int) {
	c := *a
	*a = *b
	*b = c
}

func ArrSum(arr [10]int) int {
	c := 0
	for i := 0; i < len(arr); i++ {
		c += arr[i]
	}
	return c
}

func Average(arr []int, length int) float32 {
	sum := 0
	for i := 0; i < length; i++ {
		sum += arr[i]
	}
	return (float32)(sum / length)
}

type People struct {
	name  string
	id    int
	phone string
	sex   string
}

func (people People) eat() {
	fmt.Println(people.name, " is eating ... ")
}

func (people People) sleep() {
	fmt.Println(people.name, "is sleeping ...")
}

func (people People) GetPhone() string {
	return people.phone
}

type Police struct {
	name string
	age  int
}

func (police Police) CheckPeopleInfo(people *People) {
	fmt.Println(police.name, " is checking info : ")
	fmt.Println("-------------")
	fmt.Println(people.name)
	fmt.Println(people.id)
	fmt.Println(people.phone)
	fmt.Println(people.sex)
	fmt.Println("-------------")

	people.id = 5678
	fmt.Println(people)
}

func sayhello() {
	counter := 0
	for true {
		fmt.Println(counter, ": hello world!")
		counter += 1
		time.Sleep(time.Second)
	}
}

func main() {

	go sayhello()

	for true {
		fmt.Println("main is running")
		time.Sleep(time.Second)
	}

	var a int
	a = 10
	b := 11
	c := a + b
	fmt.Println(c)

	const pi float32 = 3.1415926
	fmt.Println(pi)

	const pi_1 = 3.1415926
	fmt.Println(pi_1)

	var ptr *int = &a
	fmt.Println(*ptr)

	var ptr1 = &a
	fmt.Println(*ptr1)

	ptr2 := &a
	fmt.Println(*ptr2)

	if true {
		d := 100
		e := 1000
		fmt.Println(d + e)
	}

	for i := 0; i < 100; i++ {
		fmt.Println(100 * i)
	}

	fmt.Println(add(1000, 10000))

	fmt.Println(SumAndCompare(1, 2, 3))
	fmt.Println(SumAndCompare(1, 2, 1))

	swapnum1 := 10
	swapnum2 := 49

	swapnum3 := 100
	swapnum4 := 1000

	fmt.Println(swapnum1, swapnum2)
	swap1(swapnum1, swapnum2)
	fmt.Println(swapnum1, swapnum2)

	fmt.Println(swapnum3, swapnum4)
	swap2(&swapnum3, &swapnum4)
	fmt.Println(swapnum3, swapnum4)

	fmt.Println(CircleArea(7.89))

	var arr [10]int

	for i := 0; i < 10; i++ {
		arr[i] = i
	}

	fmt.Println(ArrSum(arr))

	for i := 0; i < 10; i++ {
		arr[i] = arr[i] * arr[i]
	}

	for i := 0; i < 10; i++ {
		fmt.Println(arr[i])
	}

	arr1 := []int{1, 2, 3, 4, 5, 6}
	fmt.Println(Average(arr1, len(arr1)))

	arr2 := [][]int{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	}

	var arr3 []int
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			arr3 = append(arr3, arr2[i][j])
		}
	}

	for i := 0; i < len(arr3); i++ {
		fmt.Println(arr3[i])
	}

	Add := func(a int, b int) int {
		return a + b
	}
	fmt.Println(Add(3, 4))

	counter := GetCounter()

	fmt.Println(counter())
	fmt.Println(counter())
	fmt.Println(counter())
	fmt.Println(counter())
	fmt.Println(counter())

	var brandon People
	brandon.name = "brandon"
	brandon.id = 123
	brandon.phone = "12323213123"
	brandon.sex = "male"

	fmt.Println(brandon)

	brandon.eat()
	fmt.Println(brandon.GetPhone())

	var police Police
	police.name = "john"
	police.age = 30

	police.CheckPeopleInfo(&brandon)

	fmt.Println(brandon)
	{
		var c = make([]float64, 3, 100)

		fmt.Println(len(c), cap(c))
		for i := 0; i < 100; i++ {
			c = append(c, (float64)(i)/(float64)(100))
		}

		fmt.Println(len(c), cap(c))
		for i := 0; i < len(c); i++ {
			fmt.Println(c[i])
		}
		sliceA := c[10:80]
		fmt.Println(sliceA)

		var sum float64 = 0
		for idx, num := range sliceA {
			sum += num
			fmt.Println(idx, sum)
		}
		fmt.Println(sum)
	}

	var namemap map[string]int

	namemap = make(map[string]int)

	namemap["johe"] = 30
	namemap["joey"] = 12
	namemap["lily"] = 3
	namemap["geo"] = 60

	fmt.Println(namemap)

	for name := range namemap {
		fmt.Println(name, namemap[name])
	}

	age, exist := namemap["brandon"]

	if exist {
		fmt.Println(age)
		delete(namemap, "brandon")
	} else {
		fmt.Println("brandon", "not in map")
		namemap["brandon"] = 29
	}

	fmt.Println(frac(10))
}
