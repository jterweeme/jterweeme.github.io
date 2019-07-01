package main

import "fmt"

func main() {
    sum := 0
    for i := 0; i < 1000; i++ {
        if i % 3 == 0 || i % 5 == 0 { sum += i }
    }
    fmt.Printf("Sum of all the multiples of 3 or 5 below 1000 = %d\n", sum)
    fmt.Printf("Oplossing 2: %d\n", problem2())
}

func problem2() int {
    var xmax, term1, term2, temp, xsum = 4000000, 1, 2, 0, 2

    for {
        temp = term1 + term2
        if temp > xmax {
            break
        }
        if temp % 2 == 0 {
            xsum += temp
        }
        term1 = term2
        term2 = temp
    }
    return xsum
    //fmt.Printf("Oplossing: %d\n", xsum)

    //return 0
}


