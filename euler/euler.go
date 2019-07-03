package main

import "fmt"
import "io/ioutil"
import "strings"
import "time"

/*
#1 If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

Antwoord: 233,168
*/

func problem1() string {
    var sum int32 = 0
    for i := int32(0); i < 1000; i++ {
        if i % 3 == 0 || i % 5 == 0 { sum += i }
    }
    var ret string = fmt.Sprintf("%d", sum);
    return ret
}

/*
#2 Each new term in the Fibonacci sequence is generated by adding the previous
two terms. By starting with 1 and 2, the first 10 terms will be:

1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

By considering the terms in the Fibonacci sequence whose values do not
exceed four million, find the sum of the even-valued terms.

Antwoord: 4,613,732
*/

func problem2() string {
    var xmax, term1, term2, temp, xsum int32 = 4000000, 1, 2, 0, 2

    for {
        temp = term1 + term2
        if temp > xmax { break }
        if temp % 2 == 0 { xsum += temp }
        term1 = term2
        term2 = temp
    }
    return fmt.Sprintf("%d", xsum)
}

/*
#3 The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
*/

func primeHelper(n uint64) uint64 {
    var presets = [4]uint64{3000,300,100,8}
    for i := 0; i < 4; i++ {
        if n > presets[i] * presets[i] { return n / presets[i] }
    }
    return 0
}

func isprime(n uint64) bool {
    if n == 0 || n == 1 { return false }
    var limit = primeHelper(n)
    for i := uint64(2); i < limit; i++ {
        if n % i == 0 { return false }
    }
    return true
}

func problem3() string {
    var n uint64 = 600851475143
    var best uint64 = 0
    for n > 1 {
        var factor uint64 = 0
        for i := uint64(2); i < 999999; i++ {    // semi oneindige loop
            if isprime(i) == false { continue }
            if n % i == 0 {
                factor = i
                break
            }
        }
        if factor > best { best = factor }
        n = n / factor
    }
    var ret string = fmt.Sprintf("%d", best)
    return ret
}

/*
#4 A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
*/

func isPalindrome(n int) bool {
    var temp, rev = n, 0
    for temp != 0 {
        rev = rev * 10 + temp % 10
        temp = temp / 10
    }
    return n == rev
}

func problem4() string {
    var best = 0
    for a := 100; a < 1000; a++ {
        for b := 100; b < 1000; b++ {
            var c = a * b
            if isPalindrome(c) && c > best { best = c }
        }
    }
    var ret string = fmt.Sprintf("%d", best)
    return ret
}

/*
#5 2520 is the smallest number that can be divided by
each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly
divisible by all of the numbers from 1 to 20?

Antwoord: 232,792,560
*/

func isdivisible(n int, lower int, max int) bool {
    for i := lower; i <= max; i++ {
        if n % i > 0 { return false }
    }
    return true
}

func problem5() string {
    var start = 2520
    var number = start
    for isdivisible(number, 11, 20) == false { number += start }
    return fmt.Sprintf("%d", number)
}

/*
#6 The sum of the squares of the first ten natural numbers is,
1^2 + 2^2 + ... + 10^2 = 385

The square of the sum of the first ten natural numbers is,
(1 + 2 + ... + 10)2 = 552 = 3025

Hence the difference between the sum of the squares of the first ten
natural numbers and the square of the sum is 3025 − 385 = 2640.

Find the difference between the sum of the squares of the
first one hundred natural numbers and the square of the sum.

Antwoord: 25,164,150
*/

func problem6() string {
    var sumsquare, squaresum = 0, 0
    for x := 1; x <= 100; x++ {
        sumsquare += x * x
        squaresum += x
    }
    var ret string = fmt.Sprintf("%d", squaresum * squaresum - sumsquare)
    return ret
}

/*
#7 By listing the first six prime numbers: 2, 3, 5,
7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?

Antwoord: 104,743
*/

func reducer7(n int) int {
    if n > 300 * 300 { return n / 300 }
    if n > 100 * 100 { return n / 100 }
    if n > 8 * 8 { return (n / 8) }
    return n
}

func problem7() string {
    var p = 3
    var sqp = reducer7(p)
    var ret = 0
    var n = 10001
    for j := 0; j < n - 1; j++ {
        for i := 2; i < sqp; i++ {
            if p % i == 0 {
                p += 1
                sqp = reducer7(p)
                i = 1
            }
        }
        ret = p
        p += 2
        sqp = reducer7(p)
    }
    return fmt.Sprintf("%d", ret)
}

/*
#8 Largest product in a series

The four adjacent digits in the 1000-digit number that
have the greatest product are 9 × 9 × 8 × 9 = 5832.

Find the thirteen adjacent digits in the 1000-digit number that
have the greatest product. What is the value of this product?

Antwoord: 23,514,624,000
*/

func problem8() string {
    const series1 string = "7316717653133062491922511967442657474235534919493496983520312774506326239578318016984801869478851843858615607891129494954595017379583319528532088055111254069874715852386305071569329096329522744304355766896648950445244523161731856403098711121722383113622298934233803081353362766142828064444866452387493035890729629049156044077239071381051585930796086670172427121883998797908792274921901699720888093776657273330010533678812202354218097512545405947522435258490771167055601360483958644670632441572215539753697817977846174064955149290862569321978468622482839722413756570560574902614079729686524145351004748216637048440319989000889524345065854122758866688116427171479924442928230863465674813919123162824586178664583591245665294765456828489128831426076900422421902267105562632111110937054421750694165896040807198403850962455444362981230987879927244284909188845801561660979191338754992005240636899125607176060588611646710940507754100225698315520005593572972571636269561882670428252483600823257530420752963450"

    var cur, best uint64 = 0, 0
    for _, chr := range series1 {
        cur = (cur % uint64(1000000000000)) * 10 + uint64(chr - '0')
        var peel, product uint64 = cur, 1
        for peel > 0 {
            product *= peel % 10
            peel = peel / 10
        }
        if product > best { best = product }
    }
    return fmt.Sprintf("%d", best)
}

/*  
#9 Special Pythagorean triplet

A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
a2 + b2 = c2

For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.

There exists exactly one Pythagorean triplet for which a + b + c = 1000.
Find the product abc.

Antwoord: 31,875,000
*/

func problem9x() int {
    for a := 1; a < 1000 - 1; a++ {
        for b := 1; b < 1000 - a; b++ {
            var c = 1000 - a - b
            if a * a + b * b == c * c {
                return a * b * c
            }
        }
    }
    return 0
}

func problem9() string {
    return fmt.Sprintf("%d", problem9x())
}

/*
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
*/

func problem10() string {
    var v [2000000]bool
    for i := 0; i < 2000000; i++ { v[i] = true }
    v[0], v[1] = false, false
    for p := 2; p * p < 2000000; p++ {
        if v[p] == true {
            for i := p * 2; i < 2000000; i += p {
                v[i] = false
            }
        }
    }
    var sum uint64 = 0
    for i := uint64(0); i < 2000000; i++ {
        if v[i] == true { sum += i }
    }
    return fmt.Sprintf("%d", sum)
}

/*
#11 Largest product in a grid

In the 20x20 grid below, four numbers along
a diagonal line have been marked in red.

The product of these numbers is 26 x 63 x 78 x 14 = 1,788,696.

What is the greatest product of four adjacent numbers in the same
direction (up, down, left, right, or diagonally) in the 20x20 grid?

Antwoord: 70,600,674
*/

func problem11() string {
    t11 := [][]uint32{
        { 8, 2,22,97,38,15, 0,40, 0,75, 4, 5, 7,78,52,12,50,77,91, 8},
        {49,49,99,40,17,81,18,57,60,87,17,40,98,43,69,48, 4,56,62, 0},
        {81,49,31,73,55,79,14,29,93,71,40,67,53,88,30, 3,49,13,36,65},
        {52,70,95,23, 4,60,11,42,69,24,68,56, 1,32,56,71,37, 2,36,91},
        {22,31,16,71,51,67,63,89,41,92,36,54,22,40,40,28,66,33,13,80},
        {24,47,32,60,99, 3,45, 2,44,75,33,53,78,36,84,20,35,17,12,50},
        {32,98,81,28,64,23,67,10,26,38,40,67,59,54,70,66,18,38,64,70},
        {67,26,20,68, 2,62,12,20,95,63,94,39,63, 8,40,91,66,49,94,21},
        {24,55,58, 5,66,73,99,26,97,17,78,78,96,83,14,88,34,89,63,72},
        {21,36,23, 9,75, 0,76,44,20,45,35,14, 0,61,33,97,34,31,33,95},
        {78,17,53,28,22,75,31,67,15,94, 3,80, 4,62,16,14, 9,53,56,92},
        {16,39, 5,42,96,35,31,47,55,58,88,24, 0,17,54,24,36,29,85,57},
        {86,56, 0,48,35,71,89, 7, 5,44,44,37,44,60,21,58,51,54,17,58},
        {19,80,81,68, 5,94,47,69,28,73,92,13,86,52,17,77, 4,89,55,40},
        { 4,52, 8,83,97,35,99,16, 7,97,57,32,16,26,26,79,33,27,98,66},
        {88,36,68,87,57,62,20,72, 3,46,33,67,46,55,12,32,63,93,53,69},
        { 4,42,16,73,38,25,39,11,24,94,72,18, 8,46,29,32,40,62,76,36},
        {20,69,36,41,72,30,23,88,34,62,99,69,82,67,59,85,74, 4,36,16},
        {20,73,35,29,78,31,90, 1,74,31,49,71,48,86,81,16,23,57, 5,54},
        { 1,70,54,71,83,51,54,69,16,92,33,48,61,43,52, 1,89,19,67,48}}

    var best uint32 = 0
    for i := 0; i < 20; i++ {
        for j := 0; j < 16; j++ {
            var prod uint32 = t11[i][j] * t11[i][j + 1] * t11[i][j+2] * t11[i][j+3]
            if prod > best { best = prod }
            prod = t11[j][i] * t11[j+1][i] * t11[j+2][i] * t11[j+3][i]
            if prod > best { best = prod }
        }
    }
    for i := 0; i < 16; i++ {
        for j := 0; j < 16; j++ {
            var prod uint32 = t11[i][j] * t11[i+1][j+1] * t11[i+2][j+2] * t11[i+3][j+3]
            if prod > best { best = prod }
        }
    }
    for i := 3; i < 20; i++ {
        for j := 0; j < 16; j++ {
            var prod uint32 = t11[i][j] * t11[i-1][j+1] * t11[i-2][j+2] * t11[i-3][j+3]
            if prod > best { best = prod }
        }
    }

    return fmt.Sprintf("%d", best)
}

/*
#12 Highly divisible triangular number

The sequence of triangle numbers is generated by adding the natural numbers.
So the 7th triangle number would be 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28. The
first ten terms would be:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

Let us list the factors of the first seven triangle numbers:

     1: 1
     3: 1,3
     6: 1,2,3,6
    10: 1,2,5,10
    15: 1,3,5,15
    21: 1,3,7,21
    28: 1,2,4,7,14,28

We can see that 28 is the first triangle number to have over five divisors.

What is the value of the first triangle number to have over five hundred divisors?

Antwoord: 76,576,500
*/

func num_divisors(n int) int {
    if n % 2 == 0 { n = n >> 1 }
    var divisors, count = 1, 0
    for n % 2 == 0 {
        count++
        n = n >> 1
    }
    divisors = divisors * (count + 1)
    var p = 3
    for n != 1 {
        count = 0
        for n % p == 0 {
            count++
            n = n / p
        }
        divisors = divisors * (count + 1)
        p += 2
    }
    return divisors
}

func find_triangular_index(factor_limit int) int {
    var n = 1
    var lnum = num_divisors(n)
    var rnum = num_divisors(n + 1)
    for lnum * rnum < factor_limit {
        lnum = rnum
        n += 1
        rnum = num_divisors(n + 1)
    }
    return n
}

func problem12() string {
    var i = find_triangular_index(500)
    return fmt.Sprintf("%d", (i * (i + 1)) >> 1)
}

/*
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

Antwoord: 5,537,376,230
*/

func isdigit(c uint8) bool {
    return c >= '0' && c <= '9'
}

func problem13() string {
    b, err := ioutil.ReadFile("euler13.txt")

    if err != nil {
        return "0"
    }

    // stop de nummers uit file in array
    var numbers2 [5000]uint64
    var n uint64 = 0
    for _, el := range b {
        if isdigit(el) {
            numbers2[n] = uint64(el) - uint64('0')
            n++
        }
    }

    // bereken totalSum
    var totalSum [100]uint64
    var sum, end uint64 = 0, 0

    for i := 50; i > 0; i-- {
        for j := 0; j < 100; j++ {
            sum += numbers2[j * 50 + (i - 1)]
        }
        totalSum[end] = sum % 10
        sum /= 10
        end++
    }
    for sum > 0 {
        totalSum[end] = sum % 10
        end++
        sum /= 10
    }

    // haal het antwoord uit totalSum
    end--
    var ret uint64 = 0
    var factor uint64 = 1000000000
    for i:= 0; i < 10; i++ {
        ret += totalSum[end] * factor
        end--
        factor /= 10
    }

    return fmt.Sprintf("%d", ret)
}



/*
#14 Longest Collatz sequence

The following iterative sequence is defined for the set of positive integers:

n -> n/2 (n is even)
n -> 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:
13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1

It can be seen that this sequence (starting at 13 and finishing at 1)
contains 10 terms. Although it has not been proved yet (Collatz Problem),
it is thought that all starting numbers finish at 1.

Which starting number, under one million, produces the longest chain?

NOTE: Once the chain starts the terms are allowed to go above one million.

Antwoord: 837,799
*/

func collatz(n uint32) uint32 {
    var count uint32 = 1
    for n > 1 {
        if n % 2 == 0 {
            n = n >> 1
        } else { n = n * 3 + 1 }
        count++
    }
    return count
}

func problem14() string {
    var lower, upper, best_start, best_length uint32 = 1, 1000000, 0, 0
    for i := lower; i < upper; i++ {
        var length uint32 = collatz(i)
        if length > best_length { best_start, best_length = i, length }
    }
    return fmt.Sprintf("%d", best_start)
}

/*
#15 Lattice paths

Starting in the top left corner of a 2x2 grid, and only being able to move
to the right and down, there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20x20 grid?

Antwoord: 137,846,528,820
*/

/*
opgelost met dynamic programming
*/

func problem15() string {
    var size, paths, i uint64 = 20, 1, 0
    for i = 0; i < size; i++ {
        paths = (paths * (2 * size - i)) / (i + 1)
    }
    return fmt.Sprintf("%d", paths)
}

/*
#16 Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Antwoord: 1,366
*/

/*
simpele implementatie van grote nummers in de largeNum
array. Deze array in een loop 1000 keer verdubbelen.
Dan de nummers in de array optellen
*/

func problem16() string {
    var largeNum [400]uint8
    largeNum[0] = 2
    var carry, sum uint64 = 0, 0
    for e := 1; e < 1000; e++ {
        for i := 0; i < 400; i++ {
            largeNum[i] *= 2
            largeNum[i] += uint8(carry)
            carry = uint64(largeNum[i]) / 10
            largeNum[i] = largeNum[i] % 10
        }
    }
    for i := 0; i < 400; i++ {
        sum += uint64(largeNum[i])
    }
    return fmt.Sprintf("%d", sum)
}

/*
#17 Number letter counts

If the numbers 1 to 5 are written out in words: one, two, three, four,
five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

If all the numbers from 1 to 1000 (one thousand) inclusive
were written out in words, how many letters would be used?

NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and
forty-two) contains 23 letters and 115 (one hundred and fifteen) contains
20 letters. The use of "and" when writing out numbers is in compliance
with British usage.

Antwoord: 21,124
*/

func problem17() string {
    arr17 := []string{"one", "two", "three", "four", "five", "six", "seven",
        "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
        "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
        "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"}
    var xsum int = 0
    for i := 0; i < 19; i++ {
        xsum += len(arr17[i])
    }
    for i := 19; i < 27; i++ {
        xsum += len(arr17[i])
        for j := 0; j < 9; j++ {
            xsum += len(arr17[i]) + len(arr17[j])
        }
    }
    for i := 0; i < 9; i++ {
        xsum += len(arr17[i]) + len("hundred")
        for j := 0; j < 19; j++ {
            xsum += len(arr17[i]) + len("hundred") + len("and") + len(arr17[j])
        }
        for j := 19; j < 27; j++ {
            xsum += len(arr17[i]) + len("hundred") + len("and") + len(arr17[j])
            for k := 0; k < 9; k++ {
                xsum += len(arr17[i]) + len("hundredand") + len(arr17[j]) + len(arr17[k])
            }
        }
    }
    xsum += len("onethousand")
    return fmt.Sprintf("%d", xsum)
}

/*
#18: Maximum path sum I

By starting at the top of the triangle below and moving to adjacent
numbers on the row below, the maximum total from top to bottom is 23.

3
7 4
2 4 6
8 5 9 3

That is, 3 + 7 + 4 + 9 = 23.

Find the maximum total from top to bottom of the triangle below:

75
95 64
17 47 82
18 35 87 10
20 04 82 47 65
19 01 23 75 03 34
88 02 77 73 07 63 67
99 65 04 28 06 16 70 92
41 41 26 56 83 40 80 70 33
41 48 72 33 47 32 37 16 94 29
53 71 44 65 25 43 91 52 97 51 14
70 11 33 28 77 73 17 78 39 68 17 57
91 71 52 38 17 14 91 43 58 50 27 29 48
63 66 04 68 89 53 67 30 73 16 69 87 40 31
04 62 98 27 23 09 70 98 73 93 38 53 60 04 23

NOTE: As there are only 16384 routes, it is possible to solve this
problem by trying every route. However, Problem 67, is the same
challenge with a triangle containing one-hundred rows; it cannot be
solved by brute force, and requires a clever method! ;o)

Antwoord: 1,074
*/

func myPow64(base uint64, e uint64) uint64 {
    if e == 0 { return 1 }
    var ret uint64 = base
    e--
    for e > 0 {
        ret *= base
        e--
    }
    return ret
}

func problem18() string {
    t18 := [][]uint64 {
        {75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {95,64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {17,47,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {18,35,87,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {20, 4,82,47,65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {19, 1,23,75, 3,34, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {88, 2,77,73, 7,63,67, 0, 0, 0, 0, 0, 0, 0, 0},
        {99,65, 4,28, 6,16,70,92, 0, 0, 0, 0, 0, 0, 0},
        {41,41,26,56,83,40,80,70,33, 0, 0, 0, 0, 0, 0},
        {41,48,72,33,47,32,37,16,94,29, 0, 0, 0, 0, 0},
        {53,71,44,65,25,43,91,52,97,51,14, 0, 0, 0, 0},
        {70,11,33,28,77,73,17,78,39,68,17,57, 0, 0, 0},
        {91,71,52,38,17,14,91,43,58,50,27,29,48, 0, 0},
        {63,66, 4,68,89,53,67,30,73,16,69,87,40,31, 0},
        { 4,62,98,27,23, 9,70,98,73,93,38,53,60, 4,23}}

    var possibilities uint64 = myPow64(2, 14)
    var best uint64 = 0

    for i := uint64(0); i <= possibilities; i++ {
        var index, sum uint64 = 0, t18[0][0]
        for j := uint64(0); j < 14; j++ {
            index = index + (i >> j & 1)
            var value uint64 = t18[j + 1][index]
            sum += value
        }
        if sum > best { best = sum }
    }
    return fmt.Sprintf("%d", best)
}

/*
#19 Counting Sundays

You are given the following information, but you may prefer to do some research for yourself.

    * 1 Jan 1900 was a Monday.
    * Thirty days has September,
      April, June and November.
      All the rest have thirty-one,
      Saving February alone,
      Which has twenty-eight, rain or shine.
      And on leap years, twenty-nine.
    * A leap year occurs on any year evenly divisible by 4, but not on a
      century unless it is divisible by 400.

How many Sundays fell on the first of the month during
the twentieth century (1 Jan 1901 to 31 Dec 2000)?

Antwoord: 171
*/

func problem19() string {
    return "0"
}

func problem20() string {
    return "0"
}

func problem21() string {
    return "0"
}

func problem22() string {
    return "0"
}

func problem23() string {
    return "0"
}

/*
#24: Lexicographic permutations

A permutation is an ordered arrangement of objects. For example, 3124 is
one possible permutation of the digits 1, 2, 3 and 4. If all of the
permutations are listed numerically or alphabetically, we call it
lexicographic order. The lexicographic permutations of 0, 1 and 2 are:

012   021   102   120   201   210

What is the millionth lexicographic permutation
of the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?

Antwoord: 2,783,915,460
*/

func problem24() string {
/*
    a := []uint8 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
    var r uint8 = 0
    var perm uint32 = 999999
    for j := uint8(0); j < 10; j++ {
        var i uint8 = perm / fac32(9 - j)
        perm = perm % fac32(9 - j)
        ret[r] = a[i] + '0'
        r++
    }
*/
    return "0"
}

func problem25() string {
    return "0"
}

/*
#26: Reciprocal cycles

A unit fraction contains 1 in the numerator. The decimal representation
of the unit fractions with denominators 2 to 10 are given:

    1/2 =  0.5
    1/3 =  0.(3)
    1/4 =  0.25
    1/5 =  0.2
    1/6 =  0.1(6)
    1/7 =  0.(142857)
    1/8 =  0.125
    1/9 =  0.(1)
    1/10 =  0.1 

Where 0.1(6) means 0.166666..., and has a 1-digit recurring
cycle. It can be seen that 1/7 has a 6-digit recurring cycle.

Find the value of d < 1000 for which 1/d contains the
longest recurring cycle in its decimal fraction part.

Antwoord: 983
*/

/*
Simpel algoritme om cycle length te bepalen, werkt niet
voor sommige nummers met cycle length 1, dan wordt er
0 teruggegeven. Dit zal de uitkomst van deze opdracht niet veranderen
*/

func cycleLength(n int) int {
    var a, t = 1, 0
    for t < n {
        a = a * 10 % n
        t++
        if a == 0 { return 0 }
        if a == 1 { return t }
    }
    return 0
}

func problem26() string {
    var best_n, best_length = 0, 0
    for i := 999; i > 1; i-- {
        var clength = cycleLength(i)
        if clength > best_length {
            best_n, best_length = i, clength
            if best_length == i - 1 { break }
        }
    }
    return fmt.Sprintf("%d", best_n)
}

/*
#27: Quadratic primes

Euler discovered the remarkable quadratic formula:

n^2 + n + 41

It turns out that the formula will produce 40 primes for the consecutive
integer values 0 <= n <= 39. However, when n = 40, 40^2 + 40 + 41 = 40(40 + 1) + 41 is
divisible by 41, and certainly when n = 41, 41^2 + 41 + 41 is clearly divisible by 41.

The incredible formula n^2 - 79n + 1601 was discovered, which produces 80
primes for the consecutive values 0 <= n <= 79. The product of the
coefficients, −79 and 1601, is −126479.

Considering quadratics of the form:

n^2 + an + b, where |a| < 1000 and |b| <= 1000

where |n| is the modulus/absolute value of n
e.g. |11| = 11 and |−4| = 4

Find the product of the coefficients, a and b, for the quadratic expression
that produces the maximum number of primes for consecutive values of n,
starting with n = 0.

Antwoord: -59231
*/

func myAbs32(n int32) int32 {
    if n < 0 { return n * -1 }
    return n
}

func problem27() string {
    var best_a, best_b, best_n int32 = 0, 0, 0
    var a, b, n int32
    for a = -999; a < 1000; a++ {
        for b = -1000; b <= 1000; b++ {
            n = 0
            for isprime(uint64(myAbs32(n * n + a * n + b))) { n++ }
            if n > best_n {
                best_a, best_b, best_n = a, b, n
            }
        }
    }
    return fmt.Sprintf("%d", best_a * best_b)
}

/*
#28 Number spiral diagonals

Starting with the number 1 and moving to the right in a
clockwise direction a 5 by 5 spiral is formed as follows:

21 22 23 24 25
20  7  8  9 10
19  6  1  2 11
18  5  4  3 12
17 16 15 14 13

It can be verified that the sum of the numbers on the diagonals is 101.

What is the sum of the numbers on the diagonals
in a 1001 by 1001 spiral formed in the same way?

Antwoord: 669,171,001
*/

func problem28() string {
    var xsum, step, foo, root int32 = 1, 2, 1, 1001
    for foo < root * root {
        for i := int32(0); i < 4; i++ {
            foo += step
            xsum += foo
        }
        step += 2
    }
    return fmt.Sprintf("%d", xsum)
}

var answers2 = [...]string{"233168", "4613732", "6857",
    "906609", "232792560", "25164150", "104743", "23514624000",
    "31875000", "142913828922", "70600674", "76576500",
    "5537376230", "837799", "137846528820",
    "1366", "21124", "1074", "171", "648", "31626", "871198282",
    "4179871", "2783915460", "4782", "983", "-59231",
    "669171001", "9183", "443839", "73682", "45228", "100",
    "40730", "55", "872187", "748317",
    "932718654", "840", "210", "7652413", "162",
    "16695334890", "5482660", "1533776805", "5777",
    "134043", "9110846700", "296962999629", "997651",
    "121313", "142857", "4075", "376", "249", "972", "153", "26241",
    "107359", "26033", "28684", "127035954683", "49", "1322", "272", "661", "7273",
    "6531031914842725", "510510", "8319823", "428570",
    "303963552391", "7295372", "402", "161667",
    "190569291", "71", "55374", "73162890", "40886", "427337", "260324", "425185",
    "101524", "2772", "1818", "1097343", "7587457", "743", "1217", "14234", "8581146",
    "1258", "518408346", "14316", "24702", "8739992577", "18769", "709", "756872327473",
    "37076114526", "228"}

func run2(p uint32) string {
    var ret string = "0"
    switch p {
    case 1:
        ret = problem1()
    case 2:
        ret = problem2()
    case 3:
        ret = problem3()
    case 4:
        ret = problem4()
    case 5:
        ret = problem5()
    case 6:
        ret = problem6()
    case 7:
        ret = problem7()
    case 8:
        ret = problem8()
    case 9:
        ret = problem9()
    case 10:
        ret = problem10()
    case 11:
        ret = problem11()
    case 12:
        ret = problem12()
    case 13:
        ret = problem13()
    case 14:
        ret = problem14()
    case 15:
        ret = problem15()
    case 16:
        ret = problem16()
    case 17:
        ret = problem17()
    case 18:
        ret = problem18()
    case 19:
        ret = problem19()
    case 20:
        ret = problem20()
    case 21:
        ret = problem21()
    case 22:
        ret = problem22()
    case 23:
        ret = problem23()
    case 24:
        ret = problem24()
    case 25:
        ret = problem25()
    case 26:
        ret = problem26()
    case 27:
        ret = problem27()
    case 28:
        ret = problem28()
    }
    return ret
}

func worker(prob_n chan uint32, done chan bool) {
    for i := range prob_n {
        start := time.Now()
        var ret string = run2(i)
        elapsed := time.Since(start)
        var status string = "OK"
        if strings.Compare(answers2[i - 1], ret) != 0 {
            status = "Error"
        }
        fmt.Printf("#%d: %s %s %s\n", i, ret, elapsed, status)
    }
    done <- true
}

const nWorkers = 4

func main() {
    start := time.Now()
/*
    for i := uint32(1); i <= 28; i++ {
        var ret string = run2(i)
        var status string = "OK"
        if strings.Compare(answers2[i - 1], ret) != 0 {
            status = "Error"
        }
        fmt.Printf("#%d: %s %s\n", i, ret, status)
    }
*/
    var prob_n = make(chan uint32, 10)
    var done = make(chan bool, 10)

    for i := 0; i < nWorkers; i++ {
        go worker(prob_n, done)
    }

    for i := 1; i <= 28; i++ {
        prob_n <- uint32(i)
    }
    close(prob_n)
    var tmp int
    for range done {
        tmp++
        if tmp == nWorkers {
            break
        }
    }

    elapsed := time.Since(start)
    fmt.Printf("Total: %s\n", elapsed)
}



