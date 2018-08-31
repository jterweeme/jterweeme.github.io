#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define bool int
#define true 1
#define false 0

void xmemcpy(void *dest, void *src, size_t n)
{   char *csrc = (char *)src;
    char *cdest = (char *)dest;
    size_t i;
    for (i = 0; i<n; i++) cdest[i] = csrc[i];
}

static uint32_t primeHelper(uint32_t n)
{   uint16_t presets[] = {3000,300,100,8};
    uint8_t i;
    for (i = 0; i < 4; i++)
        if (n > presets[i] * presets[i]) return n / presets[i];
    return n;
}

static bool isprime32(uint32_t n)
{   if (n == 0 || n == 1) return false;
    uint32_t limit = primeHelper(n), i;
    for (i = 2; i < limit; i++)
        if (n % i == 0) return false;
    return true;
}

static uint32_t myPow32(uint32_t base, uint32_t e)
{   if (e == 0) return 1;
    uint32_t ret = base;
    while (--e) ret *= base;
    return ret;
}

static uint64_t myPow64(uint64_t base, uint64_t e)
{   if (e == 0) return 1;
    uint64_t ret = base;
    while (--e) ret *= base;
    return ret;
}

static uint32_t fac32(uint32_t n)
{   uint32_t product = 1;
    while (n > 1) product *= n--;
    return product;
}

static uint8_t decimals32(uint32_t n)
{   uint8_t i = 0;
    while (n) n = n / 10, i++;
    return i;
}

static uint8_t decimals64(uint64_t n)
{   uint8_t i = 0;
    while (n) n = n / 10, i++;
    return i;
}

static uint32_t sumProperDivs1(uint32_t n)
{   uint32_t sum = 0, i;
    for (i = 1; i <= n / 2; i++)
        sum += n % i == 0 ? i : 0;
    return sum;
}

static uint32_t *sieve32(uint32_t *nr, uint32_t n)
{   uint8_t *v = malloc(n + 1);
    uint32_t i, j;
    for (i = 0; i < n; i++) v[i] = 1;
    v[0] = v[1] = 0;
    for (i = 2; i * i < n; i++)
        if (v[i] == 1)
            for (j = i * 2; j <= n; j += i)
                v[j] = 0;
    uint32_t size = 32;
    uint32_t *primes = malloc(size * 4);
    j = 0;
    for (i = 0; i < n; i++)
    {   if (v[i] == 1)
        {   primes[j++] = i;
            if (j >= size)
            {   size *= 2;
                primes = realloc(primes, size * 4);
            }
        }
    }
    free(v);
    *nr = j;
    return primes;
}

static uint32_t sieve232(uint32_t *primes, uint32_t n)
{   uint8_t *v = malloc(n + 1);
    uint32_t i, j;
    for (i = 0; i < n; i++) v[i] = 1;
    v[0] = v[1] = 0;
    for (i = 2; i * i < n; i++)
        if (v[i] == 1)
            for (j = i * 2; j <= n; j += i)
                v[j] = 0;
    for (i = 0, j = 0; i < n; i++)
        if (v[i] == 1)
            primes[j++] = i;
    free(v);
    return j - 1;
}

static const uint32_t triangle32(uint32_t n) { return n * (n + 1) >> 1; }
static const uint32_t pentagon32(uint32_t n) { return n * (3 * n - 1) / 2; }
static const uint32_t hexagon32(uint32_t n) { return n * (2 * n - 1); }

static uint32_t reverse32(uint32_t n, uint8_t base)
{   uint32_t rev = 0, temp = 0;
    for (temp = n; temp != 0; temp /= base) rev = rev * base + temp % base;
    return rev;
}

static int ispalindrome32(uint32_t n, uint8_t base)
{   return n == reverse32(n, base);
}

static uint32_t gcd(uint32_t a, uint32_t b)
{   while (b)
    {   uint32_t c = b;
        b = a % b;
        a = c;
    }
    return a;
}

static int32_t linSearch8(uint8_t *beg, uint8_t *end, uint8_t n)
{
    int32_t i = 0;
    while (beg != end)
    {
        if (*beg++ == n) return i;
        i++;
    }
    return -1;
}

static int32_t linSearch32(uint32_t *beg, uint32_t *end, uint32_t n)
{
    int32_t i = 0;
    while (beg != end)
    {
        if (*beg++ == n) return i;
        i++;
    }
    return -1;
}

static bool binSearch(uint32_t *first, uint32_t *last, uint32_t n)
{
    uint32_t *middle = (last - first) / 2 + first;
    while (first <= last)
    {   if (*middle < n) first = middle + 1;
        else if (*middle == n) return true;
        else last = middle - 1;
        middle = (last - first) / 2 + first;
    }
    return false;
}

static void swap29(uint32_t *a, uint32_t *b)
{   uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

static void sort29(uint32_t *begin, uint32_t *end)
{   uint32_t *a, *b;
    for (a = begin; a < end - 1; a++)
        for (b = begin; b < end - 1; b++)
            if (b[0] > b[1])
                swap29(b, b + 1);
}

static bool hasDigitsOnce32(uint32_t n, uint8_t *beg, uint8_t *end)
{
    while (n)
    {
        int32_t pos = linSearch8(beg, end, n % 10);
        if (pos >= 0) beg[pos] = 99;
        else return false;
        n = n / 10;
    }
    return true;
}

static void xstring32(char *s, uint32_t n)
{   if (n == 0)
    {   s[0] = '0';
        s[1] = 0;
        return;
    }
    uint8_t decs = decimals32(n);
    s[decs] = 0;
    while (n)
        s[--decs] = n % 10 + '0', n = n / 10;
}

static void xstring64(char *s, uint64_t n)
{   uint8_t decs = decimals64(n);
    s[decs] = 0;
    while (n)
        s[--decs] = n % 10 + '0', n = n / 10;
}

/*
#1 If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

Antwoord: 233,168
*/

static const uint32_t summation1(uint32_t n, uint32_t xmax)
{   uint32_t xlen = xmax / n;
    return ((xlen * (xlen + 1)) >> 1) * n;
}

static uint32_t multiples1(uint32_t limit)
{   return summation1(3, limit - 1) + summation1(5, limit - 1) - summation1(15, limit - 1);
}

static char *problem1()
{   char *ret = malloc(50);
    xstring32(ret, multiples1(1000));
    return ret;
}

/*
#2 Each new term in the Fibonacci sequence is generated by adding the previous
two terms. By starting with 1 and 2, the first 10 terms will be:

1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

By considering the terms in the Fibonacci sequence whose values do not
exceed four million, find the sum of the even-valued terms.

Antwoord: 4,613,732
*/

static uint32_t fibonacci(uint32_t xmax)
{   uint32_t term1 = 1, term2 = 2, temp = 0, xsum = 2;
    while (true)
    {   temp = term1 + term2;
        if (temp > xmax) break;
        if (temp % 2 == 0) xsum += temp;
        term1 = term2, term2 = temp;
    }
    return xsum;
}

static char *problem2()
{   char *ret = malloc(50);
    xstring32(ret, fibonacci(4000000));
    return ret;
}

/*
#3 The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
*/

/*
71 * 839 * 1471 * 6857 = 600851475143
*/

static char *problem3()
{   uint64_t n = 600851475143ULL;
    uint32_t best = 0;
    while (n > 1)
    {   uint32_t factor = 0, i;
        for (i = 2; i < 999999; i++)
        {   if (isprime32(i) == 0) continue;
            if (n % i == 0)
            {   factor = i;
                break;
            }
        }
        if (factor > best) best = factor;
        n = n / factor;
    }
    char *ret = malloc(50);
    xstring32(ret, best);
    return ret;
}

/*
#4 A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
*/

static char *problem4()
{   uint32_t best = 0;
    uint32_t a, b, c;
    for (a = 0; a < 1000; a++)
    {   for (b = 0; b < 1000; b++)
        {   c = a * b;
            if (ispalindrome32(c, 10) && c > best)
                best = c;
        }
    }
    char *ret = malloc(50);
    xstring32(ret, best);
    return ret;
}

/*
#5 2520 is the smallest number that can be divided by
each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly
divisible by all of the numbers from 1 to 20?

Antwoord: 232,792,560
*/

static int isdivisible(uint32_t n, uint32_t lower, uint32_t max)
{   uint32_t i;
    for (i = lower; i <= max; i++) if (n % i > 0) return false;
    return true;
}

static char *problem5()
{   uint32_t lower = 11, max = 20, start = 2520, number = start;
    while (isdivisible(number, lower, max) == false) number += start;
    char *ret = malloc(50);
    xstring32(ret, number);
    return ret;
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

static char *problem6()
{   uint32_t min = 1, max = 100, sumsquare = 0, squaresum = 0, i;
    for (i = min; i <= max; i++) sumsquare += i * i;
    for (i = min; i <= max; i++) squaresum += i;
    squaresum = squaresum * squaresum;
    char *ret = malloc(50);
    xstring32(ret, squaresum - sumsquare);
    return ret;
}

/*
#7 By listing the first six prime numbers: 2, 3, 5,
7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?

Antwoord: 104,743
*/

static uint32_t reducer7(uint32_t n)
{   if (n > 300 * 300) return n / 300;
    if (n > 100 * 100) return n / 100;
    if (n > 8 * 8) return n / 8;
    return n;
}

static char *problem7()
{   uint32_t n = 10001;
    uint32_t p = 3, sqp = reducer7(p), ret = 0;
    uint32_t i, j;
    for (j = 0; j < n - 1; j++)
    {   for (i = 2; i < sqp; i++)
            if (p % i == 0) sqp = reducer7(++p), i = 1;
        ret = p, p += 2, sqp = reducer7(p);
    }
    char *ret2 = malloc(50);
    xstring32(ret2, ret);
    return ret2;
}

/*
#8 Largest product in a series

The four adjacent digits in the 1000-digit number that
have the greatest product are 9 × 9 × 8 × 9 = 5832.

Find the thirteen adjacent digits in the 1000-digit number that
have the greatest product. What is the value of this product?

Antwoord: 23,514,624,000
*/

static char series1[] = "73167176531330624919225119674426574742355349194934"
        "96983520312774506326239578318016984801869478851843"
        "85861560789112949495459501737958331952853208805511"
        "12540698747158523863050715693290963295227443043557"
        "66896648950445244523161731856403098711121722383113"
        "62229893423380308135336276614282806444486645238749"
        "30358907296290491560440772390713810515859307960866"
        "70172427121883998797908792274921901699720888093776"
        "65727333001053367881220235421809751254540594752243"
        "52584907711670556013604839586446706324415722155397"
        "53697817977846174064955149290862569321978468622482"
        "83972241375657056057490261407972968652414535100474"
        "82166370484403199890008895243450658541227588666881"
        "16427171479924442928230863465674813919123162824586"
        "17866458359124566529476545682848912883142607690042"
        "24219022671055626321111109370544217506941658960408"
        "07198403850962455444362981230987879927244284909188"
        "84580156166097919133875499200524063689912560717606"
        "05886116467109405077541002256983155200055935729725"
        "71636269561882670428252483600823257530420752963450";

static char *problem8()
{   char *s = series1;
    uint64_t cur = 0, best = 0;
    while (*s)
    {   cur = (cur % 1000000000000ULL) * 10 + (*s++ - '0');
        uint64_t peel = cur, product = 1;
        while (peel) product *= peel % 10, peel = peel / 10;
        if (product > best) best = product;
    }
    char *ret = malloc(50);
    xstring64(ret, best);
    return ret;
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

static uint32_t problem9x()
{   uint32_t search = 1000;
    uint32_t a, b;
    for (a = 1; a <= search - 2; a++)
    {   for (b = 1; b <= search - 2; b++)
        {   uint32_t c = search - a - b;
            if (a * a + b * b == c * c)
                return a * b * c;
        }
    }
    return 0;
}

static char *problem9()
{
    char *ret = malloc(50);
    xstring32(ret, problem9x());
    return ret;
}

/*
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
*/

#define LIMIT10 2000000

static char *problem10()
{   uint32_t *primes = malloc(150000*4);
    uint32_t n = sieve232(primes, 2000000), i;
    uint64_t sum = 0;
    for (i = 0; i <= n; i++)
        sum += primes[i];
    free(primes);
    char *ret = malloc(50);
    xstring64(ret, sum);
    return ret;
}

/*
#11 Largest product in a grid

In the 20×20 grid below, four numbers along
a diagonal line have been marked in red.

08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08
49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00
81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65
52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91
22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80
24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50
32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70
67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21
24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72
21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95
78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92
16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57
86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58
19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40
04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66
88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69
04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36
20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16
20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54
01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48

The product of these numbers is 26 × 63 × 78 × 14 = 1788696.

What is the greatest product of four adjacent numbers in the same
direction (up, down, left, right, or diagonally) in the 20×20 grid?

Antwoord: 70,600,674
*/

uint8_t t11[20][20] = {{ 8, 2,22,97,38,15, 0,40, 0,75, 4, 5, 7,78,52,12,50,77,91, 8},
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
            { 1,70,54,71,83,51,54,69,16,92,33,48,61,43,52, 1,89,19,67,48}};

static char *problem11()
{   uint32_t best = 0;
    uint8_t i, j;
    for (i = 0; i < 20; i++)
    {   for (j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i][j+1] * t11[i][j+2] * t11[i][j+3];
            if (prod > best) best = prod;
            prod = t11[j][i] * t11[j+1][i] * t11[j+2][i] * t11[j+3][i];
            if (prod > best) best = prod;
        }
    }
    for (i = 0; i < 16; i++)
    {   for (j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i+1][j+1] * t11[i+2][j+2] * t11[i+3][j+3];
            if (prod > best) best = prod;
        }
    }
    for (i = 3; i < 20; i++)
    {   for (j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i-1][j+1] * t11[i-2][j+2] * t11[i-3][j+3];
            if (prod > best) best = prod;
        }
    }
    char *ret = malloc(50);
    xstring32(ret, best);
    return ret;
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

static uint32_t num_divisors(uint32_t n)
{   if (n % 2 == 0) n = n >> 1;
    uint32_t divisors = 1, count = 0;
    while (n % 2 == 0) count++, n = n >> 1;
    divisors = divisors * (count + 1);
    uint32_t p = 3;
    while (n != 1)
    {   count = 0;
        while (n % p == 0) count++, n = n / p;
        divisors = divisors * (count + 1), p += 2;
    }
    return divisors;
}

static uint32_t find_triangular_index(uint16_t factor_limit)
{   uint32_t n = 1, lnum = num_divisors(n), rnum = num_divisors(n + 1);
    while (lnum * rnum < factor_limit)
        lnum = rnum, rnum = num_divisors(++n + 1);
    return n;
}

static char *problem12()
{   uint32_t i = find_triangular_index(500);
    uint32_t ret = ((i * (i + 1)) >> 1);
    char *ret2 = malloc(50);
    xstring32(ret2, ret);
    return ret2;
}

/*
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

Antwoord: 5,537,376,230
*/

static char *problem13()
{
    uint8_t numbers[5000];
    FILE *fp = fopen("euler13.txt", "r");
    uint16_t i = 0, j;
    while (i < 5000)
    {
        int c = fgetc(fp);
        if (isdigit(c))
            numbers[i++] = c - '0';
    }
    fclose(fp);
    uint8_t totalSum[100];
    uint64_t sum = 0;
    uint8_t end = 0;
    for (i = 50; i > 0; i--)
    {
        for (j = 0; j < 100; j++)
            sum += numbers[j * 50 + (i - 1)];
        totalSum[end++] = sum % 10;
        sum /= 10;
    }
    for (;sum > 0; sum /= 10)
        totalSum[end++] = sum % 10;
    end--;
    char *ret = malloc(50);
    for (i = 0; i < 10; i++)
        ret[i] = (char)(totalSum[end--] + 48);
    ret[i] = 0;
    return ret;
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

static uint32_t collatz(uint32_t n)
{   uint32_t count = 1;
    while (n > 1) n = n % 2 == 0 ? n >> 1 : n * 3 + 1, count++;
    return count;
}

static char *problem14()
{
    uint32_t lower = 1, upper = 1000000;
    uint32_t best_start = 0, best_length = 0;
    uint32_t i;
    for (i = lower; i < upper; i++)
    {   uint32_t length = collatz(i);
        if (length > best_length)
            best_start = i, best_length = length;
    }
    char *ret = malloc(50);
    xstring32(ret, best_start);
    return ret;
}

/*
#15 Lattice paths

Starting in the top left corner of a 2x2 grid, and only being able to move
to the right and down, there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20×20 grid?

Antwoord: 137,846,528,820
*/

static char *problem15()
{   uint8_t size = 20;
    uint64_t paths = 1;
    uint8_t i;
    for (i = 0; i < size; i++)
        paths = (paths * (2 * size - i)) / (i + 1);
    char *ret = malloc(50);
    xstring64(ret, paths);
    return ret;
}

/*
#16 Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Antwoord: 1,366
*/

static char *problem16()
{   uint16_t e = 1000;
    uint8_t largeNum[400];
    memset(largeNum, 0, 400);
    largeNum[0] = 2;
    uint8_t carry = 0;
    uint32_t sum = 0;
    uint16_t i;
    while (--e)
    {   for (i = 0; i < sizeof(largeNum); i++)
        {   largeNum[i] *= 2;
            largeNum[i] += carry;
            carry = largeNum[i] / 10;
            largeNum[i] = largeNum[i] % 10;
        }
    }
    for (i = 0; i < sizeof(largeNum); i++)
        sum += largeNum[i];
    char *ret = malloc(50);
    xstring32(ret, sum);
    return ret;
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

static size_t len(const char *s)
{
    return strlen(s);
}

static char *problem17()
{   char arr1[][15] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
        "seventeen", "eighteen", "nineteen",
        "twenty", "thirty", "forty", "fifty", "sixty",
        "seventy", "eighty", "ninety"};
    uint32_t xsum = 0;
    uint8_t i, j;
    for (i = 0; i < 19; i++)
        xsum += len(arr1[i]);
    for (i = 19; i < 27; i++)
    {   xsum += len(arr1[i]);
        for (j = 0; j < 9; j++)
            xsum += len(arr1[i]) + len(arr1[j]);
    }
    for (i = 0; i < 9; i++)
    {   xsum += len(arr1[i]) + len("hundred");
        for (j = 0; j < 19; j++)
            xsum += len(arr1[i]) + len("hundred") + len("and") + len(arr1[j]);
        for (j = 19; j < 27; j++)
        {   xsum += len(arr1[i]) + len("hundred") + len("and") + len(arr1[j]);
            uint8_t k;
            for (k = 0; k < 9; k++)
                xsum += len(arr1[i]) + len("hundredand") + len(arr1[j]) + len(arr1[k]);
        }
    }
    xsum += len("onethousand");
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
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

static uint8_t triangle[][15] = {
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
    { 4,62,98,27,23, 9,70,98,73,93,38,53,60, 4,23}};

static char *problem18()
{   uint32_t possibilities = myPow64(2, sizeof(triangle[0]) - 1);
    uint32_t best = 0;
    uint32_t i, j;
    for (i = 0; i <= possibilities; i++)
    {   uint32_t index = 0;
        uint32_t sum = triangle[0][0];
        for (j = 0; j < sizeof(triangle[0]) - 1; j++)
        {   index = index + (i >> j & 1);
            uint8_t value = triangle[j + 1][index];
            sum += value;
        }
        if (sum > best) best = sum;
    }
    char *ret = malloc(50);
    xstring32(ret, best);
    return ret;
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

static bool isLeap(uint16_t year)
{   if (year % 4 > 0) return false;
    if (year % 100 > 0) return true;
    return false;
}

#define SUNDAY 5

static char *problem19()
{   uint8_t months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint32_t day = 0;
    uint32_t sunday_count = 0;
    uint32_t year;
    for (year = 1901; year <= 2000; year++)
    {   bool leap = isLeap(year);
        uint8_t m;
        for (m = 0; m < 12; m++)
        {   if (day % 7 == SUNDAY)
                sunday_count++;
            day += months[m];
            if (leap == true && months[m] == 28)
                day++;
        }
    }
    char *ret = malloc(50);
    xstring32(ret, sunday_count);
    return ret;
}

/*
#20 Factorial digit sum

n! means n × (n − 1) × ... × 3 × 2 × 1

For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!

Antwoord: 648
*/

static char *problem20()
{   uint8_t f = 100;
    uint16_t buf[200];
    memset(buf, 0, 400);
    buf[0] = f;
    uint8_t i, j;
    for (i = f - 1; i > 0; i--)
    {   uint16_t carry = 0;
        for (j = 0; j < 200; j++)
        {   buf[j] *= i;
            buf[j] += carry;
            carry = buf[j] / 10;
            buf[j] = buf[j] % 10;
        }
    }
    uint32_t sum = 0;
    for (i = 0; i < 200; i++) sum += buf[i];
    char *ret = malloc(50);
    xstring32(ret, sum);
    return ret;
}

/*
#21 Amicable numbers

Let d(n) be defined as the sum of proper divisors of
n (numbers less than n which divide evenly into n).
If d(a) = b and d(b) = a, where a ≠ b, then a and b are an
amicable pair and each of a and b are called amicable numbers.

For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22,
44, 55 and 110; therefore d(220) = 284. The proper divisors
of 284 are 1, 2, 4, 71 and 142; so d(284) = 220.

Evaluate the sum of all the amicable numbers under 10000.

Antwoord: 31,626
*/

/*
220 & 284
1184 & 1210
2620 & 2924
5020 & 5564
6232 & 6368
*/

static char *problem21()
{
    uint32_t low = 1, high = 10000;
    uint32_t l[high - low];
    memset(l, 0, (high - low) * 4);
    uint32_t i;
    for (i = low; i <= high; i++)
        l[i - low] = sumProperDivs1(i);
    uint32_t sum = 0;
    for (i = 0; i <= (high - low); i++)
    {   uint32_t ind = l[i];
        if (i + low < ind && low <= ind && ind <= high && l[ind - low] == i + low)
            sum += (i + low) + ind;
    }
    char *ret = malloc(50);
    xstring32(ret, sum);
    return ret;
}

/*
#22 Names scores

Using names.txt (right click and 'Save Link/Target As...'), a 46K text
file containing over five-thousand first names, begin by sorting it into
alphabetical order. Then working out the alphabetical value for each name,
multiply this value by its alphabetical position in the list to obtain a name score.

For example, when the list is sorted into alphabetical order, COLIN, which
is worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. So,
COLIN would obtain a score of 938 × 53 = 49714.

What is the total of all the name scores in the file?

Antwoord: 871,198,282
*/

static uint8_t letterwaarde(uint8_t c)
{   return c > 64 ? c - 64 : c;
}

static void swap22(char *a, char *b)
{   char tmp = *a;
    *a = *b;
    *b = tmp;
}

static char *problem22()
{   FILE *fp;
    fp = fopen("euler22.txt", "r");
    char *names = malloc(30*6000);
    memset(names, 0, 30*6000);
    int c;
    uint16_t a = 0, b = 0;
    while ((c = fgetc(fp)) != EOF)
    {   if (c == 0x0a)
        {   a++, b = 0;
            continue;
        }
        names[a * 30 + b++] = c;
    }
    fclose(fp);
    uint8_t i;
    for (a = 0; a < 5162; a++)
        for (b = 0; b < 5162; b++)
            if (strcmp(names + b * 30, names + (b + 1) * 30) > 0)
                for (i = 0; i < 30; i++)
                    swap22(names + b * 30 + i, names + (b + 1) * 30 + i);
    uint32_t total = 0;
    for (a = 0; a < 5163; a++)
    {   uint32_t score = 0;
        for (i = 0; i < 30; i++) score += letterwaarde(names[a * 30 + i]);
        score = score * (a + 1);
        total += score;
    }
    free(names);
    char *ret = malloc(50);
    xstring32(ret, total);
    return ret;
}

/*
#23 Non-abundant sums

A perfect number is a number for which the sum of its proper divisors is
exactly equal to the number. For example, the sum of the proper divisors
of 28 would be 1 + 2 + 4 + 7 + 14 = 28, which means that 28 is a perfect number.

A number n is called deficient if the sum of its proper divisors
is less than n and it is called abundant if this sum exceeds n.

As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest
number that can be written as the sum of two abundant numbers is 24. By
mathematical analysis, it can be shown that all integers greater than 28123
can be written as the sum of two abundant numbers. However, this upper limit
cannot be reduced any further by analysis even though it is known that the
greatest number that cannot be expressed as the sum of two abundant numbers
is less than this limit.

Find the sum of all the positive integers which
cannot be written as the sum of two abundant numbers.

Antwoord: 4,179,871
*/

static bool find23(uint16_t *begin, uint16_t *end, uint16_t n)
{   while (begin <= end)
    {   uint16_t *mid = (end - begin) / 2 + begin;
        if (*mid < n) begin = mid + 1;
        else if (*mid == n) return true;
        else end = mid - 1;
    }
    return false;
}

static char *problem23()
{
    uint16_t xmax = 28123;
    uint16_t abundants[28123];
    uint16_t i, j = 0;
    for (i = 1; i <= xmax; i++)
        if (sumProperDivs1(i) > i)
            abundants[j++] = i;
    j--;
    uint32_t xsum = 1;
    for (i = 2; i <= xmax; i++)
    {   bool boo = true;
        uint16_t *it;
        for (it = abundants; it != abundants + j; it++)
        {   if (*it < i)
            {   if (find23(abundants, abundants + j, i - *it))
                {   boo = false;
                    break;
                }
            } else break;
        }
        if (boo == true) xsum += i;
    }
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
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

static char *problem24()
{   uint8_t a[] = {0,1,2,3,4,5,6,7,8,9}, r = 0, j;
    char *ret = malloc(50);
    uint32_t perm = 999999;
    for (j = 0; j < 10; j++)
    {   uint8_t i = perm / fac32(9 - j);
        perm = perm % fac32(9 - j);
        ret[r++] = a[i] + '0';
        xmemcpy(a + i, a + i + 1, 9 - i);
    }
    ret[10] = 0;
    return ret;
}

/*
#25: 1000-digit Fibonacci number

The Fibonacci sequence is defined by the recurrence relation:

    Fn = Fn−1 + Fn−2, where F1 = 1 and F2 = 1.

Hence the first 12 terms will be:

    F1 = 1
    F2 = 1
    F3 = 2
    F4 = 3
    F5 = 5
    F6 = 8
    F7 = 13
    F8 = 21
    F9 = 34
    F10 = 55
    F11 = 89
    F12 = 144

The 12th term, F12, is the first term to contain three digits.

What is the index of the first term in the Fibonacci sequence to contain 1000 digits?

Antwoord: 4,782
*/

#if 0
static void xprint(uint8_t *x)
{
    printf("%u%u%u\r\n", x[0], x[1], x[2]);
}
#endif

static char *problem25()
{   uint16_t cnt = 3, i;
    uint8_t fib1[1000], fib2[1000], fib3[1000], carry = 0;
    memset(fib1, 0, 1000);
    memset(fib2, 0, 1000);
    memset(fib3, 0, 1000);
    fib1[0] = 1, fib2[0] = 1, fib3[0] = 2;
    while (fib3[999] == 0)
    {   xmemcpy(fib1, fib2, 1000);
        xmemcpy(fib2, fib3, 1000);
        for (i = 0; i < 1000; i++)
        {   fib3[i] = fib1[i] + fib2[i] + carry;
            carry = fib3[i] / 10;
            fib3[i] = fib3[i] % 10;
        }
        cnt++;
    }
    char *ret = malloc(50);
    xstring32(ret, cnt);
    return ret;
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

static uint32_t cycleLength(uint32_t n)
{   uint32_t a = 1, t = 0;
    while (t < n)
    {   a = a * 10 % n, t++;
        if (a == 0) return 0;
        if (a == 1) return t;
    }
    return 0;
}

static char *problem26()
{   uint32_t best_n = 0, best_length = 0, i;
    for (i = 999; i > 1; i--)
    {   uint32_t clength = cycleLength(i);
        if (clength > best_length)
        {   best_n = i, best_length = clength;
            if (best_length == i - 1) break;
        }
    }
    char *ret = malloc(50);
    xstring32(ret, best_n);
    return ret;
}

/*
#27: Quadratic primes

Euler discovered the remarkable quadratic formula:

n^2+n+41

It turns out that the formula will produce 40 primes for the consecutive
integer values 0≤n≤39. However, when n=40,402+40+41=40(40+1)+41 is
divisible by 41, and certainly when n=41,412+41+41

is clearly divisible by 41.

The incredible formula n2−79n+1601
was discovered, which produces 80 primes for the consecutive values 0≤n≤79

. The product of the coefficients, −79 and 1601, is −126479.

Considering quadratics of the form:

    n^2+an+b

, where |a|<1000 and |b|≤1000

where |n|
is the modulus/absolute value of n
e.g. |11|=11 and |−4|=4

Find the product of the coefficients, a
and b, for the quadratic expression that produces the maximum
number of primes for consecutive values of n, starting with n=0.

Antwoord: -59231
*/

static char *problem27()
{   int32_t best_a = 0, best_b = 0, best_n = 0, a, b;
    for (a = -999; a < 1000; a++)
    {   for (b = -1000; b <= 1000; b++)
        {   int32_t n = 0;
            while (isprime32(abs(n * n + a * n + b))) n++;
            if (n > best_n) best_a = a, best_b = b, best_n = n;
        }
    }
    char *ret = malloc(50);
    snprintf(ret, 50, "%d", best_a * best_b);
    return ret;
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

static char *problem28()
{   uint32_t root = 1001;
    uint32_t xsum = 1, step, foo;
    uint8_t i;
    for (step = 2, foo = 1; foo < root * root; step += 2)
        for (i = 0; i < 4; i++) foo += step, xsum += foo;
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
}

/*
#29: Distinct powers

Consider all integer combinations of a^b for 2 ≤ a ≤ 5 and 2 ≤ b ≤ 5:

22=4, 23=8, 24=16, 25=32
32=9, 33=27, 34=81, 35=243
42=16, 43=64, 44=256, 45=1024
52=25, 53=125, 54=625, 55=3125

If they are then placed in numerical order, with any repeats
removed, we get the following sequence of 15 distinct terms:

4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125

How many distinct terms are in the sequence
generated by ab for 2 <= a <= 100 and 2 <= b <= 100?

Antwoord: 9,183
*/

static uint32_t myRoot(uint32_t n)
{   uint8_t a;
    uint32_t e, b;
    for (a = 2; a <= 10; a++)
        for (e = 1, b = 0; (b = myPow32(a, e)) <= 100; e++)
            if (b == (n & 0xffff0000) >> 16) return a << 16 | e;
    return n;
}

static uint32_t eqpow(uint32_t n)
{   uint32_t root = myRoot(n);
    return (root & 0xffff0000) | (n & 0xffff) * (root & 0xffff);
}

static uint32_t ndistinct(uint32_t *begin, uint32_t *end)
{   uint32_t ret = 1;
    uint32_t previous = *begin++;
    while (begin != end)
    {   uint32_t cur = *begin++;
        if (cur != previous) ret++;
        previous = cur;
    }
    return ret;
}

static char *problem29()
{   uint32_t buf[99*99];
    uint16_t a, b;
    for (a = 2; a <= 100; a++)
        for (b = 2; b <= 100; b++)
            buf[(a-2)*99+(b-2)] = eqpow(a << 16 | b);
    sort29(buf, buf + 99*99);
    char *ret = malloc(50);
    xstring32(ret, ndistinct(buf, buf + 99*99));
    return ret;
}

/*
#30: Digit fifth powers

Surprisingly there are only three numbers that can
be written as the sum of fourth powers of their digits:

1634 = 1^4 + 6^4 + 3^4 + 4^4
8208 = 8^4 + 2^4 + 0^4 + 8^4
9474 = 9^4 + 4^4 + 7^4 + 4^4

As 1 = 14 is not a sum it is not included.

The sum of these numbers is 1634 + 8208 + 9474 = 19316.

Find the sum of all the numbers that can be written
as the sum of fifth powers of their digits.

Antwoord: 443,839
*/

/*
4,150 + 4,151 + 54,748 + 92,727 + 93,084 + 194,979 = 443,83
*/

static bool test30(uint32_t n, uint8_t p)
{   uint32_t xsum = 0, tmp = n;
    while (tmp > 0) xsum += myPow32(tmp % 10, p), tmp = tmp / 10;
    return xsum == n;
}

static char *problem30()
{   uint8_t p = 5;
    uint32_t xsum = 0, i;
    for (i = 2; i < 1000000; i++) xsum += test30(i, p) ? i : 0;
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
}

/*
#31 Coin sums

In England the currency is made up of pound, P, and pence,
p, and there are eight coins in general circulation:

    1p, 2p, 5p, 10p, 20p, 50p, P1 (100p) and P2 (200p).

It is possible to make £2 in the following way:

    1×P1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p

How many different ways can P2 be made using any number of coins?

Antwoord: 73,682
*/

static char *problem31()
{   uint8_t target = 200, coins[] = {1,2,5,10,20,50,100,200};
    uint32_t ways[target + 1];
    memset(ways, 0, (target + 1) * 4);
    ways[0] = 1;
    uint8_t i, j;
    for (i = 0; i < sizeof(coins); i++)
        for (j = coins[i]; j <= target; j++)
            ways[j] += ways[j - coins[i]];
    char *ret = malloc(50);
    xstring32(ret, ways[target]);
    return ret;
}

/*
#32: Pandigital products

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once; for example, the 5-digit number, 15234,
is 1 through 5 pandigital.

The product 7254 is unusual, as the identity, 39 × 186 = 7254, containing
multiplicand, multiplier, and product is 1 through 9 pandigital.

Find the sum of all products whose multiplicand/multiplier/product identity
can be written as a 1 through 9 pandigital.
HINT: Some products can be obtained in more than one way so be sure to only
include it once in your sum.

Antwoord: 45,228
*/

/*
4*1738 + 4*1963 + 12*483 + 18*297 + 0*27*198 + 28*157 + 39*186 + 0*42*138 + 48*159 =
6952 + 7852 + 5796 + 5346 + 0*5346 + 4396 + 7254 + 0*5796 + 7632 =
45,228
*/

static char *problem32()
{
    uint32_t *st = malloc(500000);
    uint32_t i, j, k = 0, start;
    for (i = 2; i < 60; i++)
    {
        start = i < 10 ? 1234 : 123;
        for (j = start; j < 10000/i; j++)
        {
            uint8_t nset[9], n;
            for (n = 1; n <= 9; n++) nset[n - 1] = n;
            if (hasDigitsOnce32(i, nset, nset + 9) == false) continue;
            if (hasDigitsOnce32(j, nset, nset + 9) == false) continue;
            if (hasDigitsOnce32(i * j, nset, nset + 9) == false) continue;
            st[k++] = i * j;
        }
    }
    sort29(st, st + k);
    uint32_t previous = 0, xsum = 0, *it;
    for (it = st; it != st + k; it++)
    {
        if (*it != previous)
            xsum += *it;
        previous = *it;
    }
    
    char *ret = malloc(50);
    xstring32(ret, xsum);
    free(st);
    return ret;
}

/*
#33: Digit cancelling fractions

The fraction 49/98 is a curious fraction, as an inexperienced
mathematician in attempting to simplify it may incorrectly believe
that 49/98 = 4/8, which is correct, is obtained by cancelling the 9s.

We shall consider fractions like, 30/50 = 3/5, to be trivial examples.

There are exactly four non-trivial examples of this type of fraction, less
than one in value, and containing two digits in the numerator and denominator.

If the product of these four fractions is given in its
lowest common terms, find the value of the denominator.

Antwoord: 100
*/

static char *problem33()
{   uint32_t dp = 1, np = 1, c, d, n;
    for (c = 1; c < 10; c++)
    {   for (d = 1; d < c; d++)
        {   for (n = 1; n < d; n++)
                if ((n * 10 + c) * d == (c * 10 + d) * n)
                    np *= n, dp *= d;
        }
    }
    char *ret = malloc(50);
    xstring32(ret, dp / gcd(np, dp));
    return ret;
}

/*
#34: Digit factorials

145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.

Antwoord: 40,730
*/

static uint32_t factorials34[] = { 1,1,2,6,24,120,720,5040,40320,362880};

static uint32_t facsumdig34(uint32_t n)
{   uint32_t sum = 0;
    while (n > 0) sum += factorials34[n % 10], n = n / 10;
    return sum;
}

static char *problem34()
{   uint32_t totalSum = 0, k;
    for (k = 10; k < factorials34[9] * 7; k++)
        if (facsumdig34(k) == k) totalSum += k;
    char *ret = malloc(50);
    xstring32(ret, totalSum);
    return ret;
}

/*
#35: Circular primes

The number, 197, is called a circular prime because all rotations
of the digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

How many circular primes are there below one million?

Antwoord: 55
*/

/*
2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, 97, 113, 131, 197, 199, 311,
337, 373, 719, 733, 919, 971, 991, 1193, 1931, 3119, 3779, 7793, 7937, 9311,
9377, 11939, 19391, 19937, 37199, 39119, 71993, 91193, 93719, 93911, 99371,
193939, 199933, 319993, 331999, 391939, 393919, 919393, 933199, 939193,
939391, 993319, 999331
*/

static uint32_t rotate(uint32_t n)
{   uint32_t length = decimals32(n);
    uint32_t digit = n % 10;
    return n / 10 + digit * myPow32(10, length - 1);
}

static bool iscircular(uint32_t n, uint32_t *begin, uint32_t *end)
{   uint8_t decs = decimals32(n), i = 0;
    for (i = 0; i < decs; i++)
    {   n = rotate(n);
        if (binSearch(begin, end, n) == false) return false;
    }
    return true;
}

static char *problem35()
{   uint32_t i, ncount = 0, *primes = malloc(80000*4);
    uint32_t n = sieve232(primes, 999999);
    for (i = 0; i < n; i++)
        if (iscircular(primes[i], primes, primes + n )) ncount++;
    char *ret = malloc(50);
    xstring32(ret, ncount);
    free(primes);
    return ret;
}

/*
#36: Double-base palindromes

The decimal number, 585 = 1001001001_2 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base, may not include leading zeros.)

Antwoord: 872,187
*/

static char *problem36()
{   uint32_t min = 1, limit = 1000000, xsum = 0, i;
    for (i = min; i < limit; i++)
        if (ispalindrome32(i, 10) && ispalindrome32(i, 2)) xsum += i;
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
}

/*
#37: Truncatable primes

The number 3797 has an interesting property. Being prime itself, it is possible
to continuously remove digits from left to right, and remain prime at each
stage: 3797, 797, 97, and 7. Similarly we can work from right to left: 3797, 379, 37, and 3.

Find the sum of the only eleven primes that are both
truncatable from left to right and right to left.

NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.

Antwoord: 748,317
*/

/*
23 + 37 + 53 + 73 + 313 + 317 + 373 + 797 + 3,137 + 3,797 + 739,397 = 748,317
*/

static bool isrighttruncatable(uint32_t prime, uint32_t *begin, uint32_t *end)
{   while (prime > 10)
    {   prime = prime / 10;
        if (binSearch(begin, end, prime) == false) return false;
    }
    return true;
}

static uint32_t truncate_left(uint32_t n)
{   uint8_t exp = decimals32(n) - 1;
    return n % myPow32(10, exp);
}

static bool islefttruncatable(uint32_t prime, uint32_t *begin, uint32_t *end)
{   uint8_t length = decimals32(prime), i;
    for (i = 0; i < length; i++)
    {   if (binSearch(begin, end, prime) == false) return false;
        prime = truncate_left(prime);
    }
    return true;
}

static char *problem37()
{
    uint32_t *primes = malloc(80000*4), end = 0, xsum = 0, *it;
    end = sieve232(primes, 999999);
    for (it = primes; it != primes + end; it++)
    {   if (*it == 2 || *it == 3 || *it == 5 || *it == 7)
            continue;
        if (islefttruncatable(*it, primes, primes + end) &&
                isrighttruncatable(*it, primes, primes + end))
        {
            xsum += *it;
        }
    }
    free(primes);
    char *ret = malloc(50);
    xstring32(ret, xsum);
    return ret;
}

/*
#38: Pandigital multiples

Take the number 192 and multiply it by each of 1, 2, and 3:

    192 × 1 = 192
    192 × 2 = 384
    192 × 3 = 576

By concatenating each product we get the 1 to 9 pandigital, 192384576. We
will call 192384576 the concatenated product of 192 and (1,2,3)

The same can be achieved by starting with 9 and multiplying by 1, 2, 3, 4,
and 5, giving the pandigital, 918273645, which is the concatenated product
of 9 and (1,2,3,4,5).

What is the largest 1 to 9 pandigital 9-digit number that can be formed as
the concatenated product of an integer with (1,2, ... , n) where n > 1?

Antwoord: 932,718,654
*/

static bool isPandigital(uint32_t n)
{
    uint8_t len = decimals32(n), i;
    uint8_t *nset = malloc(len);
    for (i = 1; i <= len; i++) nset[i - 1] = i;
    bool ret = hasDigitsOnce32(n, nset, nset + len);
    free(nset);
    return ret;
}

static uint32_t opdracht38()
{
    uint32_t i;
    for (i = 9387; i > 9234; i--)
    {
        uint32_t result = 2 * i + i * 100000;
        if (isPandigital(result)) return result;
    }
    return 0;
}

static char *problem38()
{
    char *ret = malloc(50);
    xstring32(ret, opdracht38());
    return ret;
}

/*
#39: Integer right triangles

If p is the perimeter of a right angle triangle with integral length
sides, {a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

Antwoord: 840
*/

/*
(40, 399, 401),
(56, 390, 394),
(105, 360, 375),
(120, 350, 370),
(140, 336, 364),
(168, 315, 357),
(210, 280, 350),
(240, 252, 348),
(252, 240, 348),
(280, 210, 350),
(315, 168, 357),
(336, 140, 364),
(350, 120, 370),
(360, 105, 375),
(390, 56, 394),
(399, 40, 401)
*/

static char *problem39()
{   uint32_t best_p = 0, best_solutions = 0, p, a;
    for (p = 100; p <= 1000; p += 2)
    {   uint32_t solutions = 0;
        for (a = 2; a < p / 3; a++)
            solutions += (p * (p - 2 * a) % (2 * (p - a)) == 0) ? 1 : 0;
        if (solutions > best_solutions) best_solutions = solutions, best_p = p;
    }
    char *ret = malloc(50);
    xstring32(ret, best_p);
    return ret;
}

/*
#40: Champernowne's constant

An irrational decimal fraction is created by concatenating the positive integers:

0.123456789101112131415161718192021...

It can be seen that the 12th digit of the fractional part is 1.

If dn represents the nth digit of the fractional
part, find the value of the following expression.

d1 × d10 × d100 × d1000 × d10000 × d100000 × d1000000

Antwoord: 210
*/

// 1*1*5*3*7*2*1=210

static uint32_t getDigit40(uint32_t i)
{   uint32_t offset = 0, decimals = 1, setLow = 1, setLength = 9, limit, x;
    for (limit = 9; i >= limit; limit += setLength * decimals)
        offset = limit, decimals++, setLow *= 10, setLength *= 10;
    uint32_t n = (i - offset) / decimals + setLow, ind = (i - offset) % decimals;
    for (x = 0; x < decimals - (ind + 1); x++) n /= 10;
    return n % 10;
}

static char *problem40()
{   uint32_t product = 1, i;
    for (i = 1; i <= 1000000; i *= 10) product *= getDigit40(i - 1);
    char *ret = malloc(50);
    xstring32(ret, product);
    return ret;
}

/*
#41: Pandigital prime

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once. For example, 2143 is a 4-digit pandigital
and is also prime.

What is the largest n-digit pandigital prime that exists?

Antwoord: 7,652,413
*/

static char *problem41()
{
    uint32_t *primes = malloc(600000*4), end = 0, best = 0, *it;
    end = sieve232(primes, 7654321);
    for (it = primes; it != primes + end; it++)
        if (isPandigital(*it) && *it > best) best = *it;
    free(primes);
    char *ret = malloc(50);
    xstring32(ret, best);
    return ret;
}

/*
#42: Coded triangle numbers

The nth term of the sequence of triangle numbers is given by,
tn = 1/2n(n+1); so the first ten triangle numbers are:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

By converting each letter in a word to a number corresponding to its
alphabetical position and adding these values we form a word value. For
example, the word value for SKY is 19 + 11 + 25 = 55 = t10. If the word
value is a triangle number then we shall call the word a triangle word.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text
file containing nearly two-thousand common English words, how many are
triangle words?

Antwoord: 162
*/

static uint32_t wordcount(char *w)
{   uint32_t count = 0;
    while (*w) count += *w++ - 64;
    return count;
}

static char *problem42()
{
    FILE *fp = fopen("euler42.txt", "r");
    char *words = malloc(2000*40);
    memset(words, 0, 2000*40);
    uint32_t triangles[20], i = 0, j = 0;
    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == 0x0a)
        {
            if (i == 0) continue;
            j++;
            words[j * 40 + i] = 0;
            i = 0;
            continue;
        }
        words[j * 40 + i] = c;
        i++;
    }
    fclose(fp);
    uint32_t cnt = 0;
    for (i = 0; i < 20; i++) triangles[i] = triangle32(i);
    for (i = 0; i < j; i++)
        if (linSearch32(triangles, triangles + 20, wordcount(words + i * 40)) >= 0)
            cnt++;
    char *ret = malloc(50);
    xstring32(ret, cnt);
    free(words);
    return ret;
}

/*
#43: Sub-string divisibility

The number, 1406357289, is a 0 to 9 pandigital number because it is made
up of each of the digits 0 to 9 in some order, but it also has a rather
interesting sub-string divisibility property.

Let d1 be the 1st digit, d2 be the 2nd digit, and
so on. In this way, we note the following:

d2d3d4=406 is divisible by 2
d3d4d5=063 is divisible by 3
d4d5d6=635 is divisible by 5
d5d6d7=357 is divisible by 7
d6d7d8=572 is divisible by 11
d7d8d9=728 is divisible by 13
d8d9d10=289 is divisible by 17

Find the sum of all 0 to 9 pandigital numbers with this property.

Antwoord: 16,695,334,890
*/

static char *problem43()
{   char *ret = malloc(50);
    xstring32(ret, 0);
    return ret;
}

/*
#44: Pentagon numbers

Pentagonal numbers are generated by the formula,
Pn=n(3n−1)/2. The first ten pentagonal numbers are:

1, 5, 12, 22, 35, 51, 70, 92, 117, 145, ...

It can be seen that P4 + P7 = 22 + 70 = 92 = P8. However,
their difference, 70 − 22 = 48, is not pentagonal.

Find the pair of pentagonal numbers, Pj and Pk, for which
their sum and difference are pentagonal and D = |Pk − Pj|
is minimised; what is the value of D?

Antwoord: 5,482,660
*/

static uint32_t opdracht44()
{
    uint32_t lpgs[9998], i, j;
    for (i = 1; i <= 9998; i++) lpgs[i - 1] = pentagon32(i);
    for (i = 0; i < 9998; i++)
        for (j = i; j < 9998; j++)
            if (binSearch(lpgs + j, lpgs + 9997, lpgs[i] + lpgs[j]) &&
                binSearch(lpgs, lpgs + 9997, lpgs[j] - lpgs[i]))
                return lpgs[j] - lpgs[i];
    return 0;
}

static char *problem44()
{
    char *ret = malloc(50);
    xstring32(ret, opdracht44());
    return ret;
}

/*
#45: Triangular, pentagonal, and hexagonal

Triangle, pentagonal, and hexagonal numbers are generated by the following formulae:
Triangle     Tn=n(n+1)/2     1, 3, 6, 10, 15, ...
Pentagonal     Pn=n(3n−1)/2     1, 5, 12, 22, 35, ...
Hexagonal     Hn=n(2n−1)     1, 6, 15, 28, 45, ...

It can be verified that T285 = P165 = H143 = 40755.

Find the next triangle number that is also pentagonal and hexagonal.

Antwoord: 1,533,776,805
*/

/*
T55,385 = P31,977 = H27,693 = 1,533,776,805
*/

static uint32_t opdracht45()
{   uint32_t vp[31834], vh[45856], i, *it;
    for (i = 166; i < 32000; i++) vp[i - 166] = pentagon32(i);
    for (i = 144; i < 46000; i++) vh[i - 144] = hexagon32(i);
    for (it = vh; it != vh + 45856; it++)
        if (linSearch32(vp, vp + 31834, *it) > -1) return *it;
    return 0;
}

static char *problem45()
{
    char *ret = malloc(50);
    xstring32(ret, opdracht45());
    return ret;
}

/*
#46: Goldbach's other conjecture

It was proposed by Christian Goldbach that every odd composite
number can be written as the sum of a prime and twice a square.

9 = 7 + 2×1^2
15 = 7 + 2×2^2
21 = 3 + 2×3^2
25 = 7 + 2×3^2
27 = 19 + 2×2^2
33 = 31 + 2×1^2

It turns out that the conjecture was false.

What is the smallest odd composite that cannot be
written as the sum of a prime and twice a square?

Antwoord: 5,777
*/

static char *problem46()
{
    char *ret = malloc(50);
    xstring32(ret, 0);
    return ret;
}

/*
#47: Distinct primes factors

The first two consecutive numbers to have two distinct prime factors are:

14 = 2 × 7
15 = 3 × 5

The first three consecutive numbers to have three distinct prime factors are:

644 = 2^2 × 7 × 23
645 = 3 × 5 × 43
646 = 2 × 17 × 19.

Find the first four consecutive integers to have four distinct
prime factors each. What is the first of these numbers?

Antwoord: 134,043
*/

/*
interpreted from https://blog.dreamshire.com/project-euler-47/
*/

static uint32_t opdracht47()
{
    uint32_t L = 300000, nf = 4, ns = 4;
    uint32_t f[L], c = 0, n, i;
    memset(f, 0, 4*L);
    for (n = 2; n < L - ns; n++)
    {   if (f[n] == nf) { if (++c == ns) return n - ns + 1; }
        else { c = 0; if (f[n] == 0) for (i = n; i < L; i += n) f[i]++; }
    }
    return 0;
}

static char *problem47()
{
    char *ret = malloc(50);
    xstring32(ret, opdracht47());
    return ret;
}

/*
#48: Self powers
*/

static char *problem48()
{   uint64_t result = 0, modulo = 10000000000ULL;
    uint16_t i, j;
    for (i = 1; i <= 1000; i++)
    {   uint64_t temp = i;
        for (j = 1; j < i; j++) temp *= i, temp %= modulo;
        result += temp, result %= modulo;
    }
    char *ret = malloc(50);
    xstring64(ret, result);
    return ret;
}

/*
#49: Prime permutations

The arithmetic sequence, 1487, 4817, 8147, in which each of the terms
increases by 3330, is unusual in two ways: (i) each of the three terms
are prime, and, (ii) each of the 4-digit numbers are permutations of one another.

There are no arithmetic sequences made up of three 1-, 2-, or 3-digit
primes, exhibiting this property, but there is one other 4-digit
increasing sequence.

What 12-digit number do you form by concatenating the three terms in this sequence?

Antwoord: 296,962,999,629
*/

static bool sameDigs32(uint32_t a, uint32_t b)
{
    uint32_t n = decimals32(a), i = 0;
    if (decimals32(b) != n) return false;
    uint8_t *digits = malloc(n);
    for (;a ; a = a / 10) digits[i++] = a % 10;
    bool ret = hasDigitsOnce32(b, digits, digits + n);
    free(digits);
    return ret;
}

static uint64_t check(uint32_t *begin, uint32_t *end)
{   uint32_t *it;
    for (it = begin; it != end; it++)
    {   if (linSearch32(begin, end, *it + 3330) && linSearch32(begin, end, *it + 6660) &&
            sameDigs32(*it, *it + 3330) && sameDigs32(*it, *it + 6660))
        {
            return (*it + 6660) + (*it + 3330) * 10000 + (uint64_t)*it * 100000000;
        }
    }
    return 0;
}

static char *problem49()
{
    //printf("%u\r\n", sameDigs32(12391, 11329));
    uint32_t end, begin = 0;
    uint32_t *primes = sieve32(&end, 9999);
    while (primes[begin++] < 1487);
    char *ret = malloc(50);
    xstring64(ret, check(primes + begin, primes + (end - 1)));
    free(primes);
    return ret;
}

/*
#50: Consecutive prime sum

The prime 41, can be written as the sum of six consecutive primes:
41 = 2 + 3 + 5 + 7 + 11 + 13

This is the longest sum of consecutive primes that adds to a prime below one-hundred.

The longest sum of consecutive primes below one-thousand
that adds to a prime, contains 21 terms, and is equal to 953.

Which prime, below one-million, can be written as the sum of the most consecutive primes?

Antwoord: 997,651
*/

static char *problem50()
{
    uint32_t limit = 1000000;
    uint32_t *primes = malloc(80000*4), end = 0;
    end = sieve232(primes, limit);
    uint32_t best_prime = 0, best_sum = 0, i, j, k;
    for (i = 0; i < end; i++)
    {   for (j = i + best_sum; j < end; j++)
        {   uint32_t xsum = 0;
            for (k = i; k <= j; k++) xsum += primes[k];
            if (xsum >= limit) break;
            uint32_t sublen = (j + 1) - i;
            if (binSearch(primes, primes + end, xsum) && sublen > best_sum)
                best_sum = sublen, best_prime = xsum;
        }
    }
    free(primes);
    char *ret = malloc(50);
    xstring32(ret, best_prime);
    return ret;
}

static char answers2[][50] = {"233168", "4613732", "6857",
    "906609", "232792560", "25164150", "104743", "23514624000",
    "31875000", "142913828922", "70600674", "76576500", "5537376230", "837799", "137846528820",
    "1366", "21124", "1074", "171", "648", "31626", "871198282",
    "4179871", "2783915460", "4782", "983", "-59231",
    "669171001", "9183", "443839", "73682", "45228", "100", "40730", "55", "872187", "748317",
    "932718654", "840", "210", "7652413", "162", "16695334890", "5482660", "1533776805", "5777",
    "134043", "9110846700", "296962999629", "997651",
    "121313", "142857", "4075", "376", "249", "972", "153", "26241", "0", "0", "0",
    "127035954683"};

static char *run(uint32_t p)
{
    switch (p)
    {
    case 1: return problem1();
    case 2: return problem2();
    case 3: return problem3();
    case 4: return problem4();
    case 5: return problem5();
    case 6: return problem6();
    case 7: return problem7();
    case 8: return problem8();
    case 9: return problem9();
    case 10: return problem10();
    case 11: return problem11();
    case 12: return problem12();
    case 13: return problem13();
    case 14: return problem14();
    case 15: return problem15();
    case 16: return problem16();
    case 17: return problem17();
    case 18: return problem18();
    case 19: return problem19();
    case 20: return problem20();
    case 21: return problem21();
    case 22: return problem22();
    case 23: return problem23();
    case 24: return problem24();
    case 25: return problem25();
    case 26: return problem26();
    case 27: return problem27();
    case 28: return problem28();
    case 29: return problem29();
    case 30: return problem30();
    case 31: return problem31();
    case 32: return problem32();
    case 33: return problem33();
    case 34: return problem34();
    case 35: return problem35();
    case 36: return problem36();
    case 37: return problem37();
    case 38: return problem38();
    case 39: return problem39();
    case 40: return problem40();
    case 41: return problem41();
    case 42: return problem42();
    case 43: return problem43();
    case 44: return problem44();
    case 45: return problem45();
    case 46: return problem46();
    case 47: return problem47();
    case 48: return problem48();
    case 49: return problem49();
    case 50: return problem50();
    }
    return 0;
}

static void runjob(uint32_t p)
{
    time_t begin = time(0);
    char *answer = run(p);
    time_t end = time(0);
    if (strcmp(answer, answers2[p - 1]) != 0)
        printf("error!");
    printf("#%u: %s %lus\r\n", p, answer, end - begin);
    free(answer);
}

int main()
{
    time_t begin = time(0);
    uint8_t i;
    for (i = 1; i <= 50; i++)
        runjob(i);
    time_t end = time(0);
    printf("Total: %lus\r\n", end - begin);
    return 0;
}

