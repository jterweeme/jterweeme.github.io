"""
Project Euler
"""

"""
Common functions
"""

def sieve(limit):
    a = [True] * limit
    a[0] = a[1] = False
    for i, isprime in enumerate(a):
        if isprime:
            yield i
            for n in range(i * i, limit, i):
                a[n] = False

def isprime(n, presets = (3000, 300, 100, 8)):
    def reducer2(n, presets = (300, 100, 8)):
        for preset in presets:
            if n > preset * preset:
                return n // preset
        return n
    if n < 2: return False
    for i in range(2, reducer2(n, presets)):
        if n % i == 0: return False
    return True

def digits(n):
    while n: yield n % 10; n = n // 10;

def decimals(n):
    i = 0
    while n: n = n // 10; i += 1
    return i

def digitalroot(n):
    while decimals(n) > 1:
        n = sum(digits(n))
    return n

triangle = lambda n: n * (n + 1) >> 1
square = lambda n: n ** 2
pentagon = lambda n: n * (3 * n - 1) >> 1
hexagon = lambda n: n * (2 * n - 1)
heptagon = lambda n: n * (5 * n - 3) >> 1
octagon = lambda n: n * (3 * n - 2)

def polygonizer(limit, n = 1):
    step, ret = 1, 0
    while ret < limit:
        yield ret
        ret += step
        step += n

def last(gen):
    xlast = 0
    for value in gen:
        xlast = value
    return xlast

ispolygon = lambda n, size: last(polygonizer(n + 1, size)) == n
istriangle = lambda n: ispolygon(n, 1)
ispentagon = lambda n: ispolygon(n, 3)
ishexagon = lambda n: ispolygon(n, 4)
isheptagon = lambda n: ispolygon(n, 5)
isoctagon = lambda n: ispolygon(n, 6)

def issquare(n):
    if n % 10 in [2,3,7,8]: return False
    if n % 3 in [2,3,5,6,8]: return False
    return ispolygon(n, 2)

def issquare2(lprimes, n):
    lpf = list(primefactors(lprimes, n))
    for freq in frequencies(lpf):
        if freq % 2 != 0: return False
    return True

""" binary search """
def issquare3(n):
    def recurse(n, lower, upper):
        range2 = upper - lower
        root = lower + range2 // 2
        if root * root == n: return True
        if lower == upper: return False  # end of options, but no match
        if root * root > n: return recurse(n, lower, root)
        return recurse(n, root + 1, upper)
    if n % 10 in [2,3,7,8]: return False
    return recurse(n, 1, n - 1)

def floorsqrt(n):
    return len(list(polygonizer(n + 1, 2))) - 1

def binSearch(d, n):
    if n > d[-1]: return False
    first, last = 0, len(d)
    middle = (first + last) >> 1
    while first <= last:
        if d[middle] < n: first = middle + 1
        elif d[middle] == n: return True
        else: last = middle - 1
        middle = (first + last) >> 1
    return False

def digit(n, i):
    return n // 10**i % 10

def product(l):
    xsum = 1
    for n in l: xsum *= n
    return xsum

def factorial(n):
    return product(range(2, n + 1))

def combinations(n, r):
    return factorial(n) // (factorial(r) * factorial(n - r))

def reverse(n, base = 10):
    temp, rev = n, 0
    while temp != 0:
        rev = rev * base + temp % base
        temp = temp // base
    return rev

def ispalindrome(n, base = 10):
    return n == reverse(n, base)

def hasOnce(n, nset):
    for i in n:
        if i in nset: nset.remove(i)
        else: return False
    return True

def hasDigitsOnce(n, nset):
    return hasOnce(digits(n), nset)

def sameDigs(a, b):
    return hasDigitsOnce(b, list(digits(a)))

def isPandigital(n):
    return hasDigitsOnce(n, list(range(1, decimals(n) + 1)))

def kounter(hand):
    dct = dict()
    for card in hand:
        if card in dct: dct[card] += 1
        else: dct[card] = 1
    return dct

def frequencies(hand):
    return (x[1] for x in kounter(hand).items())

def primefactor(primes, n):
    for prime in primes:
        if n % prime == 0: return prime
    raise Exception("Ran out of primes!")

def primefactors(primes, n):
    while n > 1:
        factor = primefactor(primes, n)
        yield factor
        n = n // factor

def primefactor2(n):
    for i in range(2, n + 1):
        if isprime(i) == False: continue
        if n % i == 0: return i
    return 0

def primefactors2(n):
    while n > 1:
        factor = primefactor2(n)
        yield factor
        n = n // factor

""" yields proper divisors """
def properDivs(n):
    for i in range(1, n // 2 + 1):
        if n % i == 0: yield i

""" yields all divisors (proper divisors + number itself) """
def divisors(n):
    for div in properDivs(n): yield div
    yield n

""" number of all divisors """
def n_divs1(n):
    if n <= 2: return n
    return len(list(divisors(n)))

""" number of all divisors using prime factorization """
def n_divs2(lprimes, n):
    if n <= 2: return n
    pfactors = primefactors(lprimes, n)
    return product(x + 1 for x in frequencies(pfactors))

""" sum of proper divisors """
def sumProperDivs1(n):
    return sum(properDivs(n))

""" sum of all divisors """
def sum_divs1(n):
    return sumProperDivs1(n) + n

""" sum of all divisors using prime factorization """
def sum_divs2(lprimes, n):
    previous, ret, current = 0, 1, 1
    for p in primefactors(lprimes, n):
        if p != previous:
            ret *= sum_divs1(current)
            current = 1
        current *= p
        previous = p
    return ret * sum_divs1(current)

""" sum of all divisors using prime factorization """
def sum_divs3(lprimes, n):
    return product(sum_divs1(key ** value)
        for key, value in kounter(primefactors(lprimes, n)).items())

""" sum of proper divisors using prime factorization """
def sumProperDivs2(lprimes, n):
    return sum_divs2(lprimes, n) - n

def fibonacci(xmax, term1 = 1, term2 = 2):
    yield term1
    yield term2
    while term1 + term2 <= xmax:
        yield term1 + term2
        term1, term2 = term2, term1 + term2

def miller_rabin_pass(a, s, d, n):
    a_to_power = pow(a, d, n)
    if a_to_power == 1:
        return True
    for i in range(s - 1):
        if a_to_power == n - 1:
            return True
        a_to_power = (a_to_power * a_to_power) % n
    return a_to_power == n - 1

def miller_rabin(n):
    if n < 2: return False
    if n in {2, 7, 61}: return True
    d = n - 1
    s = 0
    while d % 2 == 0:
        d >>= 1
        s += 1
    for a in {2,7,61}:
        if not miller_rabin_pass(a, s, d, n):
            return False
    return True

def permutations(pool):
    n = len(pool)
    c = [0] * n
    yield(list(pool))
    i = 0
    while i < n:
        if c[i] < i:
            if i % 2 == 0: pool[0], pool[i] = pool[i], pool[0]
            else: pool[c[i]], pool[i] = pool[i], pool[c[i]]
            yield(list(pool))
            c[i] += 1
            i = 0
        else:
            c[i] = 0
            i += 1

def permutations2(iterable, r=None):
    pool = tuple(iterable)
    n = len(pool)
    r = n if r is None else r
    if r > n:
        return
    indices = list(range(n))
    cycles = list(range(n, n-r, -1))
    yield tuple(pool[i] for i in indices[:r])
    while n:
        for i in reversed(range(r)):
            cycles[i] -= 1
            if cycles[i] == 0:
                indices[i:] = indices[i+1:] + indices[i:i+1]
                cycles[i] = n - i
            else:
                j = cycles[i]
                indices[i], indices[-j] = indices[-j], indices[i]
                yield tuple(pool[i] for i in indices[:r])
                break
        else:
            return

def combinations2(iterable, r):
    pool = tuple(iterable)
    n = len(pool)
    if r > n:
        return
    indices = list(range(r))
    yield tuple(pool[i] for i in indices)
    while True:
        for i in reversed(range(r)):
            if indices[i] != i + n - r:
                break
        else:
            return
        indices[i] += 1
        for j in range(i+1, r):
            indices[j] = indices[j-1] + 1
        yield tuple(pool[i] for i in indices)

def count2(start=0, step=1):
    n = start
    while True:
        yield n
        n += step

def tee2(iterable, n=2):
    it = iter(iterable)
    deques = [collections.deque() for i in range(n)]
    def gen(mydeque):
        while True:
            if not mydeque:             # when the local deque is empty
                try:
                    newval = next(it)   # fetch a new value and
                except StopIteration:
                    return
                for d in deques:        # load it to all the deques
                    d.append(newval)
            yield mydeque.popleft()
    return tuple(gen(d) for d in deques)

def gcd(a, b):
    while b: a, b = b, a % b
    return a

def ways(target, coins):
    ways = [1] + [0]*target
    for coin in coins:
        for i in range(coin, target + 1):
            ways[i] += ways[i - coin]
    return ways[target]

def concat2(lst):
    ret = 0
    for n in lst:
        e = decimals(n) if n else 1
        ret *= 10**e
        ret += n
    return ret

def testSuite():
    lprimes1 = list(sieve(500))
    lprimes2 = list()
    for n in range(500):
        if m_r(n):
            lprimes2.append(n)
    return lprimes2
    if lprimes1 != lprimes2:
        raise("Error 1")

"""
#1 Multiples of 3 and 5

If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

Antwoord: 233,168
"""

def problem1(limit = 1000):
    return sum(set(range(3,limit,3)) | set(range(5,limit,5)))

"""
#2 Even Fibonacci numbers

Each new term in the Fibonacci sequence is generated by adding the previous
two terms. By starting with 1 and 2, the first 10 terms will be:

1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

By considering the terms in the Fibonacci sequence whose values do not
exceed four million, find the sum of the even-valued terms.

Antwoord: 4,613,732
"""

"""
2 + 8 + 34 + 144 + 610 + 2,584 + 10,946 + 46,368 + 196,418 + 832,040 + 3,524,578 = 4,613,732
"""

def problem2(xmax = 4*10**6):
    return sum(x for x in fibonacci(xmax) if x % 2 == 0)

"""
#3 Largest prime factor

The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
"""

"""
71*839*1,471*6,857=600,851,475,143
"""

def problem3(n = 600851475143):
    return max(primefactors2(n))

"""
#4 Largest palindrome product

A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 x 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
"""

def problem4(x = range(100, 1000), y = range(999, 99, -1)):
    best = 0
    for a in x:
        for b in y:
            c = a * b
            if ispalindrome(c) and c > best:
                best = c
    return best

"""
#5 Smallest multiple

2520 is the smallest number that can be divided by
each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly
divisible by all of the numbers from 1 to 20?

Antwoord: 232,792,560
"""

def problem5(l = (11,12,13,14,15,16,17,18,19,20)):
    def isdivisible(n, l):
        for x in l:
            if n % x > 0:
                return False
        return True
    start = 2520
    number = start
    while isdivisible(number, l) == False:
        number += start
    return number

"""
#6 Sum square difference

The sum of the squares of the first ten natural numbers is,
1^2 + 2^2 + ... + 10^2 = 385

The square of the sum of the first ten natural numbers is,
(1 + 2 + ... + 10)2 = 552 = 3025

Hence the difference between the sum of the squares of the first ten
natural numbers and the square of the sum is 3025 - 385 = 2640.

Find the difference between the sum of the squares of the
first one hundred natural numbers and the square of the sum.

Antwoord: 25,164,150
"""

def problem6(r = range(1, 101)):
    sumsquare, squaresum = 0, 0
    for x in r:
        sumsquare += x ** 2;
        squaresum += x;
    return squaresum ** 2 - sumsquare

"""
#7 10001st prime

By listing the first six prime numbers: 2, 3, 5,
7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?

Antwoord: 104,743
"""

def problem7(n = 10001):
    def reducer2(n, presets = (300, 100, 8)):
        for preset in presets:
            if n > preset * preset:
                return n // preset
        return n
    p = 3
    sqp = reducer2(p)
    ret = 0
    for j in range(0, n - 1):
        i = 2
        while i < sqp:
            if p % i == 0:  #not a prime
                p += 1
                sqp = reducer2(p)
                i = 1
            i += 1
        ret = p
        p += 2
        sqp = reducer2(p)
    return ret

"""
#8 Largest product in a series

The four adjacent digits in the 1000-digit number that
have the greatest product are 9 x 9 x 8 x 9 = 5832.

Find the thirteen adjacent digits in the 1000-digit number that
have the greatest product. What is the value of this product?

Antwoord: 23,514,624,000
"""

series8 = ("73167176531330624919225119674426574742355349194934"
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
    "71636269561882670428252483600823257530420752963450")

def problem8(series1 = series8):
    series3 = [ord(x) - ord('0') for x in series1]
    return max(product(series3[i:i+13]) for i in range(0, len(series3) - 13))

"""
#9 Special Pythagorean triplet

A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
a2 + b2 = c2

For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.

There exists exactly one Pythagorean triplet for which a + b + c = 1000.
Find the product abc.

Antwoord: 31,875,000
"""

def problem9(search = 1000):
    for a in range(1,search - 1):
        for b in range(1,search - a):
            c = search - a - b
            if a * a + b * b == c * c:
                return a * b * c
    raise Exception("Answer not found")
    return 0

"""
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
"""

def problem10(limit = 2*10**6):
    return sum(sieve(limit))

"""
#11 Largest product in a grid

In the 20x20 grid below, four numbers along
a diagonal line have been marked in red.

The product of these numbers is 26 x 63 x 78 x 14 = 1788696.

What is the greatest product of four adjacent numbers in the same
direction (up, down, left, right, or diagonally) in the 20x20 grid?

Antwoord: 70,600,674
"""

t11 = (( 8, 2,22,97,38,15, 0,40, 0,75, 4, 5, 7,78,52,12,50,77,91, 8),
       (49,49,99,40,17,81,18,57,60,87,17,40,98,43,69,48, 4,56,62, 0),
       (81,49,31,73,55,79,14,29,93,71,40,67,53,88,30, 3,49,13,36,65),
       (52,70,95,23, 4,60,11,42,69,24,68,56, 1,32,56,71,37, 2,36,91),
       (22,31,16,71,51,67,63,89,41,92,36,54,22,40,40,28,66,33,13,80),
       (24,47,32,60,99, 3,45, 2,44,75,33,53,78,36,84,20,35,17,12,50),
       (32,98,81,28,64,23,67,10,26,38,40,67,59,54,70,66,18,38,64,70),
       (67,26,20,68, 2,62,12,20,95,63,94,39,63, 8,40,91,66,49,94,21),
       (24,55,58, 5,66,73,99,26,97,17,78,78,96,83,14,88,34,89,63,72),
       (21,36,23, 9,75, 0,76,44,20,45,35,14, 0,61,33,97,34,31,33,95),
       (78,17,53,28,22,75,31,67,15,94, 3,80, 4,62,16,14, 9,53,56,92),
       (16,39, 5,42,96,35,31,47,55,58,88,24, 0,17,54,24,36,29,85,57),
       (86,56, 0,48,35,71,89, 7, 5,44,44,37,44,60,21,58,51,54,17,58),
       (19,80,81,68, 5,94,47,69,28,73,92,13,86,52,17,77, 4,89,55,40),
       ( 4,52, 8,83,97,35,99,16, 7,97,57,32,16,26,26,79,33,27,98,66),
       (88,36,68,87,57,62,20,72, 3,46,33,67,46,55,12,32,63,93,53,69),
       ( 4,42,16,73,38,25,39,11,24,94,72,18, 8,46,29,32,40,62,76,36),
       (20,69,36,41,72,30,23,88,34,62,99,69,82,67,59,85,74, 4,36,16),
       (20,73,35,29,78,31,90, 1,74,31,49,71,48,86,81,16,23,57, 5,54),
       ( 1,70,54,71,83,51,54,69,16,92,33,48,61,43,52, 1,89,19,67,48))

def problem11(data = t11):
    best = 0
    for i in range(0, 20):
        for j in range(0, 16):
            prod = data[i][j] * data[i][j+1] * data[i][j+2] * data[i][j+3]
            best = max(prod, best)
            prod = data[j][i] * data[j+1][i] * data[j+2][i] * data[j+3][i]
            best = max(prod, best)
    for i in range(0, 16):
        for j in range(0, 16):
            prod = data[i][j] * data[i+1][j+1] * data[i+2][j+2] * data[i+3][j+3]
            best = max(prod, best)
    for i in range(3, 20):
        for j in range(0, 16):
            prod = data[i][j] * data[i-1][j+1] * data[i-2][j+2] * data[i-3][j+3]
            best = max(prod, best)
    return best

"""
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
"""

def problem12(ndivs = 500):
    lprimes = list(sieve(10**5))
    for n in polygonizer(10**8, 1):
        if n_divs2(lprimes, n) > ndivs: return n
    raise Exception("Answer not found")
    return 0

"""
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

Antwoord: 5,537,376,230
"""

def problem13(fn = "euler13.txt"):
    return sum(int(n) for n in open(fn).read().splitlines() if len(n) > 0) // 10**42
            
"""
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
"""

def problem14(limit = 10**6):
    def collatz(num, lut):
        count, n = 1, num
        while n > 1:
            if len(lut) >= n and lut[n - 1] > 0:
                return count + lut[n - 1]
            n = n >> 1 if n % 2 == 0 else n * 3 + 1
            count += 1
        return count
    lut = [0] * limit
    best_start, best_length = 0, 0
    for n in range(1, limit):
        length = collatz(n, lut)
        lut[n - 1] = length
        if length > best_length:
            best_start, best_length = n, length
    return best_start

"""
#15 Lattice paths

Starting in the top left corner of a 2x2 grid, and only being able to move
to the right and down, there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20x20 grid?

Antwoord: 137,846,528,820
"""

def problem15(size = 20):
    return combinations(2*size, size)

"""
#16 Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Antwoord: 1366
"""

def problem16(e = 1000):
    return sum(digits(2**e))

"""
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
"""

def problem17():
    def xlen(word):     # to insert possible debugging messages
        return len(word)
    xsum = 0
    arr1 = ("one", "two", "three", "four", "five", "six", "seven", "eight", "nine")
    arr2 = ("ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
        "seventeen", "eighteen", "nineteen")
    arr3 = ("twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety")
    for word in arr1:
        xsum += xlen(word)
    for word in arr2:
        xsum += xlen(word)
    for pre in arr3:
        xsum += xlen(pre)
        for word in arr1:
            xsum += xlen(pre) + xlen(word)
    for a in arr1:
        xsum += xlen(a) + xlen("hundred")
        for b in arr1:
            xsum += xlen(a) + xlen("hundred") + xlen("and") + xlen(b)
        for b in arr2:
            xsum += xlen(a) + xlen("hundred") + xlen("and") + xlen(b)
        for c in arr3:
            xsum += xlen(a) + xlen("hundred") + xlen("and") + xlen(c)
            for d in arr1:
                xsum += xlen(a) + xlen("hundred") + xlen("and") + xlen(c) + xlen(d)
    xsum+= xlen("one") + xlen("thousand")
    return xsum

"""
#18: Maximum path sum I

By starting at the top of the triangle below and moving to adjacent
numbers on the row below, the maximum total from top to bottom is 23.

3
7 4
2 4 6
8 5 9 3

That is, 3 + 7 + 4 + 9 = 23.

Find the maximum total from top to bottom of the triangle below:

NOTE: As there are only 16384 routes, it is possible to solve this
problem by trying every route. However, Problem 67, is the same
challenge with a triangle containing one-hundred rows; it cannot be
solved by brute force, and requires a clever method! ;o)

Antwoord: 1,074
"""

def problem18(fn = "euler18.txt", root = 15):
    triangle2 = [int(n) for n in open(fn).read().split()]
    while root > 1:
        for i in range(root - 1):
            j = triangle(root - 2) + i
            k = triangle(root - 1) + i
            triangle2[j] += max(triangle2[k], triangle2[k + 1])
        root -= 1
    return triangle2[0]

"""
#19 Counting Sundays

You are given the following information, but
you may prefer to do some research for yourself.

  * 1 Jan 1900 was a Monday.
  * Thirty days has September,
    April, June and November.
    All the rest have thirty-one,
    Saving February alone,
    Which has twenty-eight, rain or shine.
    And on leap years, twenty-nine.
  * A leap year occurs on any year evenly divisible by 4,
    but not on a century unless it is divisible by 400.

How many Sundays fell on the first of the month during
the twentieth century (1 Jan 1901 to 31 Dec 2000)?

Antwoord: 171
"""

def problem19():
    def isLeap(year):
        if year % 4 > 0: return False
        if year % 100 > 0: return True
        return False
    months = (31,28,31,30,31,30,31,31,30,31,30,31)
    days = ('Tuesday','Wednesday', 'Thursday','Friday','Saturday', 'Sunday', 'Monday')
    day = 0
    sunday_count = 0
    for year in range(1901,2001):
        leap = isLeap(year)
        for m in months:
            dayName = days[day%7]
            if dayName == "Sunday":
                sunday_count += 1
            day += m
            if leap == True and m == 28:
                day += 1
    return sunday_count


"""
#20 Factorial digit sum

n! means n x (n - 1) x ... x 3 x 2 x 1

For example, 10! = 10 x 9 x ... x 3 x 2 x 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!

Antwoord: 648
"""

def problem20(f = 100):
    return sum(digits(factorial(f)))

"""
#21 Amicable numbers

Let d(n) be defined as the sum of proper divisors of
n (numbers less than n which divide evenly into n).
If d(a) = b and d(b) = a, where a != b, then a and b are an
amicable pair and each of a and b are called amicable numbers.

For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22, 44,
55 and 110; therefore d(220) = 284. The proper divisors of 284 are 1, 2, 4,
71 and 142; so d(284) = 220.

Evaluate the sum of all the amicable numbers under 10000.

Antwoord: 31,626
"""

"""
220 - 284
1184 - 1210
2620 - 2924
5020 - 5564
6232 - 6368
"""

def problem21(low = 1, high = 10**4):
    l = [sumProperDivs1(i) for i in range(low, high + 1)]
    xsum = 0
    for i in range(high - low + 1):
        ind = l[i]
        if i + low < ind and low <= ind and ind <= high and l[ind - low] == i + low:
            xsum += (i + low) + ind
    return xsum

"""
#22 Names scores

Using names.txt (right click and 'Save Link/Target As...'), a 46K text
file containing over five-thousand first names, begin by sorting it into
alphabetical order. Then working out the alphabetical value for each name,
multiply this value by its alphabetical position in the list to obtain a name score.

For example, when the list is sorted into alphabetical order, COLIN, which
is worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. So,
COLIN would obtain a score of 938 x 53 = 49714.

What is the total of all the name scores in the file?

Antwoord: 871,198,282
"""

def problem22():
    names = [name for name in open("euler22.txt").read().splitlines() if len(name) > 0]
    names.sort()
    total = 0
    for i in range(0, len(names)):
        score = 0
        for letter in names[i]:
            score += ord(letter) - 64
        score = score * (i + 1)
        total += score
    return total

"""
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
"""

"""
Upper bound is actually 20,161
"""

def problem23():
    isabundant = lambda lprimes, n: sumProperDivs2(lprimes, n) > n
    lprimes = list(sieve(99999))
    lAbundants = set(x for x in range(12, 28123 + 1) if isabundant(lprimes, x) == True)
    sums = 1
    for i in range(2, 28123 + 1):
        boo = True
        for k in lAbundants:
            if k < i:
                if (i-k) in lAbundants:
                    boo = False
                    break
            else:
                break
        if boo == True:
            sums += i
    return sums

"""
#24: Lexicographic permutations

A permutation is an ordered arrangement of objects. For example, 3124 is
one possible permutation of the digits 1, 2, 3 and 4. If all of the
permutations are listed numerically or alphabetically, we call it
lexicographic order. The lexicographic permutations of 0, 1 and 2 are:

012   021   102   120   201   210

What is the millionth lexicographic permutation
of the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?

Antwoord: 2,783,915,460
"""

def problem24(pool = [0,1,2,3,4,5,6,7,8,9], perm = 1000000 - 1):
    lst = list()
    while len(pool) > 0:
        i, perm = divmod(perm, factorial(len(pool) - 1))
        lst.append(pool[i])
        pool.pop(i)
    return sum(n*10**i for i, n in enumerate(reversed(lst)))

"""
#25: 1000-digit Fibonacci number

The Fibonacci sequence is defined by the recurrence relation:

    Fn = Fn-1 + Fn-2, where F1 = 1 and F2 = 1.

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
"""

def problem25(limit = 10**999):
    return len(list(fibonacci(limit))) + 2

"""
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
"""

"""
def cycleLength(n):
    a, t = 1, 0
    while t < n:
        a = a * 10 % n
        t += 1
        if a == 0: return 0
        if a == 1: return t
    return -1
"""

"""
https://www.xarg.org/puzzle/project-euler/problem-26/
"""

def problem26b():
    def cycleLength(n):
        a, t = 1, 0
        while t < n:
            a = a * 10 % n
            t += 1
            if a == 0: return 0
            if a == 1: return t
        return -1
    def fullReptend(genx):
        for prime in genx:
            if cycleLength(prime) == prime - 1:
                yield prime
    return max(fullReptend(sieve(1000)))

def problem26():
    def cycleLength(n):
        a, t = 1, 0
        while t < n:
            a = a * 10 % n
            t += 1
            if a == 0: return 0
            if a == 1: return t
        return -1
    best_n, best_length = 0, 0
    for i in range(999, 0, -1):
        clength = cycleLength(i)
        if clength > best_length:
            best_n, best_length = i, clength
            if best_length == i - 1:
                break
    return best_n

"""
#27: Quadratic primes

Euler discovered the remarkable quadratic formula:

n^2+n+41

It turns out that the formula will produce 40 primes for the consecutive
integer values 0<=n<=39. However, when n=40,402+40+41=40(40+1)+41 is
divisible by 41, and certainly when n=41,412+41+41

is clearly divisible by 41.

The incredible formula n2-79n+1601
was discovered, which produces 80 primes for the consecutive values 0<=n<=79

. The product of the coefficients, -79 and 1601, is -126479.

Considering quadratics of the form:

    n^2+an+b

, where |a|<1000 and |b|<=1000

where |n|
is the modulus/absolute value of n
e.g. |11|=11 and |-4|=4

Find the product of the coefficients, a
and b, for the quadratic expression that produces the maximum
number of primes for consecutive values of n, starting with n=0.

Antwoord: -59231
"""

def problem27():
    best_a = best_b = best_n = 0
    sprimes = set(sieve(17000))
    for a in range(-999, 1000):
        for b in range(-1000, 1000 + 1):
            n = 0
            while abs(n * n + a * n + b) in sprimes:
                n += 1
            if n > best_n:
                best_a, best_b, best_n = a, b, n
    return best_a * best_b

"""
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
"""

"""
def genCorners(root):
    yield 1
    corner, step = 1, 2
    while corner < root * root:
        for i in range(4):
            corner += step
            yield corner
        step += 2
"""

def problem28(root = 1001):
    def genRings(root):
        yield 1
        ring, step = 24, 52
        for i in range(root >> 1):
            yield ring
            ring += step
            step += 32
    return sum(genRings(root))

"""
#29: Distinct powers

Consider all integer combinations of a^b for 2 <= a <= 5 and 2 <= b <= 5:

2^2=4, 2^3=8, 2^4=16, 2^5=32
3^2=9, 3^3=27, 3^4=81, 3^5=243
4^2=16, 4^3=64, 4^4=256, 4^5=1024
5^2=25, 5^3=125, 5^4=625, 5^5=3125

If they are then placed in numerical order, with any repeats
removed, we get the following sequence of 15 distinct terms:

4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125

How many distinct terms are in the sequence
generated by ab for 2 <= a <= 100 and 2 <= b <= 100?

Antwoord: 9,183
"""

def problem29(ar = range(2, 100+1), br = range(2,100+1)):
    def genPowers(ar, br):
        for a in ar:
            for b in br:
                yield a ** b
    return len(set(genPowers(ar, br)))

"""
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
"""

"""
4,150 + 4,151 + 54,748 + 92,727 + 93,084 + 194,979 = 443,839
"""

def problem30(p = 5):
    test = lambda n, p: n if sum(d**p for d in digits(n)) == n else 0
    return sum(test(i, p) for i in range(2, 10**6))

"""
#31: Coin sums

In England the currency is made up of pound, P, and pence,
p, and there are eight coins in general circulation:

    1p, 2p, 5p, 10p, 20p, 50p, P1 (100p) and P2 (200p).

It is possible to make P2 in the following way:

    1xP1 + 1x50p + 2x20p + 1x5p + 1x2p + 3x1p

How many different ways can P2 be made using any number of coins?

Antwoord: 73,682
"""

def problem31(target = 200, coins = (1,2,5,10,20,50,100,200)):
    return ways(target, coins)

"""
#32: Pandigital products

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once; for example, the 5-digit number, 15234,
is 1 through 5 pandigital.

The product 7254 is unusual, as the identity, 39 x 186 = 7254, containing
multiplicand, multiplier, and product is 1 through 9 pandigital.

Find the sum of all products whose multiplicand/multiplier/product
identity can be written as a 1 through 9 pandigital.
HINT: Some products can be obtained in more than one
way so be sure to only include it once in your sum.

Antwoord: 45,228
"""

"""
4*1738 + 4*1963 + 12*483 + 18*297 + 0*27*198 + 28*157 + 39*186 + 0*42*138 + 48*159 =
6952 + 7852 + 5796 + 5346 + 0*5346 + 4396 + 7254 + 0*5796 + 7632 =
6952 + 7852 + 5796 + 5346 + 4396 + 7254 + 7632 =
45,228
"""

def problem32():
    def panProducts():
        for i in range(2,  60):
            start = 1234 if i < 10 else 123
            for j in range(start, 10**4//i):
                digs = list(range(1, 10))
                if not hasDigitsOnce(i, digs): continue
                if not hasDigitsOnce(j, digs): continue
                if not hasDigitsOnce(i * j, digs): continue
                yield i * j
    return sum(set(panProducts()))

"""
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
"""

"""
16/64 = 1/4, 19/95 = 1/5, 26/65 = 2/5, 49/98 = 4/8
4*5*5*8/8 = 100
"""

def problem33():
    dp, np = 1,1
    for c in range(1,10):
        for d in range(1,c):
            for n in range(1,d):
                if (n * 10 + c) * d == (c * 10 + d) * n:
                    np *= n
                    dp *= d
    return dp // gcd(np, dp)

"""
#34: Digit factorials

145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.

Antwoord: 40,730
"""

"""
145 + 40585 = 40730
"""

def problem34():
    factorials = (1,1,2,6,24,120,720,5040,40320,362880);
    facsumdig = lambda n: sum(factorials[x] for x in digits(n));
    return sum(k for k in range(10, factorials[9] * 7) if facsumdig(k) == k)

"""
#35: Circular primes

The number, 197, is called a circular prime because all rotations
of the digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100: 2, 3,
5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

How many circular primes are there below one million?

Antwoord: 55
"""

"""
2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, 97, 113, 131, 197, 199, 311,
337, 373, 719, 733, 919, 971, 991, 1193, 1931, 3119, 3779, 7793, 7937, 9311,
9377, 11939, 19391, 19937, 37199, 39119, 71993, 91193, 93719, 93911, 99371,
193939, 199933, 319993, 331999, 391939, 393919, 919393, 933199, 939193,
939391, 993319, 999331
"""

""" number of rotations is equal to the number of digits """

def problem35(xmax = 10**6-1):
    def iscircular(n, lst):
        def rotations(n):
            def rotate(n):
                length = decimals(n)
                n, digit = divmod(n, 10)
                n += digit * 10**(length - 1)
                return n
            for i in range(0, decimals(n)):
                n = rotate(n)
                yield n
        for rotation in rotations(n):
            if rotation not in lst:
                return False
        return True
    primes = set(sieve(xmax))
    return sum(1 for p in primes if iscircular(p, primes))

"""
#36: Double-base palindromes

The decimal number, 585 = 1001001001_2 (binary),
is palindromic in both bases.

Find the sum of all numbers, less than one million,
which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in
either base, may not include leading zeros.)

Antwoord: 872,187
"""

"""
1 + 3 + 5 + 7 + 9 + 33 + 99 + 313 + 585 + 717 + 7447 + 9009 +
15351 + 32223 + 39993 + 53235 + 53835 + 73737 + 585585 = 872187
"""

def problem36(r = range(1,10**6)):
    return sum(i for i in r if ispalindrome(i, 10) and ispalindrome(i, 2))

"""
#37: Truncatable primes

The number 3797 has an interesting property. Being prime itself, it is possible
to continuously remove digits from left to right, and remain prime at each
stage: 3797, 797, 97, and 7. Similarly we can work from right to left: 3797, 379, 37, and 3.

Find the sum of the only eleven primes that are both
truncatable from left to right and right to left.

NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.

Antwoord: 748,317
"""

"""
23 + 37 + 53 + 73 + 313 + 317 + 373 + 797 + 3,137 + 3,797 + 739,397 = 748,317
"""

def problem37():
    def right(prime, primes):
        while prime > 10:
            prime = prime // 10
            if prime not in primes: return False
        return True
    def left(prime, primes):
        def truncate_left(n):
            exp = decimals(n) - 1
            return n % 10**exp
        for i in range(0, decimals(prime)):
            if prime not in primes: return False
            prime = truncate_left(prime)
        return True
    primes = set(sieve(999999))
    return sum(p for p in primes if left(p, primes) and right(p, primes)) - sum([2,3,5,7])

"""
#38: Pandigital multiples

Take the number 192 and multiply it by each of 1, 2, and 3:

192 x 1 = 192
192 x 2 = 384
192 x 3 = 576

By concatenating each product we get the 1 to 9 pandigital, 192384576. We
will call 192384576 the concatenated product of 192 and (1,2,3)

The same can be achieved by starting with 9 and multiplying by 1, 2, 3, 4,
and 5, giving the pandigital, 918273645, which is the concatenated product
of 9 and (1,2,3,4,5).

What is the largest 1 to 9 pandigital 9-digit number that can be formed as
the concatenated product of an integer with (1,2, ... , n) where n > 1?

Antwoord: 932,718,654
"""

"""
9,327*10^5 + 2*9,327 = 9,327*10^5 + 18,654 = 932,718,654
"""

def problem38():
    for i in range(9387, 9234, -1):
        result = concat2([i, 2 * i])
        if isPandigital(result):
            return result
    raise Exception("Answer not found")
    return 0

"""
#39: Integer right triangles

If p is the perimeter of a right angle triangle with integral length
sides, {a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p <= 1000, is the number of solutions maximised?

Antwoord: 840
"""

"""
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
"""

def problem39():
    best_p, best_solutions = 0, 0
    for p in range(100, 1000+1, 2):
        solutions = 0
        for a in range(2, p//3):
            if p*(p-2*a) % (2*(p-a)) == 0:
                solutions += 1
        if solutions > best_solutions:
            best_solutions = solutions
            best_p = p
    return best_p

"""
#40: Champernowne's constant

An irrational decimal fraction is created
by concatenating the positive integers:

0.123456789101112131415161718192021...

It can be seen that the 12th digit of the fractional part is 1.

If dn represents the nth digit of the fractional
part, find the value of the following expression.

d1 x d10 x d100 x d1000 x d10000 x d100000 x d1000000

Antwoord: 210
"""

"""
1*1*5*3*7*2*1=210
"""

def problem40(indices = [0,9,99,999,9999,99999,999999]):
    def champernowne(i):
        offset, decimals, setLow, setLength, limit = 0, 1, 1, 9, 9
        while i >= limit:
            offset = limit
            decimals += 1
            setLow *= 10
            setLength *= 10
            limit += setLength * decimals
        n, ind = divmod(i - offset, decimals)
        return digit(n + setLow, decimals - (ind + 1))
    return product(champernowne(i) for i in indices)

"""
#41: Pandigital prime

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once. For example, 2143 is a 4-digit pandigital
and is also prime.

What is the largest n-digit pandigital prime that exists?

Antwoord: 7,652,413
"""

"""
divisibility rule vertelt ons dat pandigitale getallen van negen of acht
decimalen lang niet priem kunnen zijn, daarom is de limiet 7654321.
"""

def problem41(limit = 7654321):
    return max(p for p in sieve(limit) if isPandigital(p))

"""
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
"""

def problem42(fn = "euler42.txt"):
    words = [w for w in open(fn).read().splitlines() if len(w) > 0]
    triangles = set(polygonizer(999, 1))   # 999 is arbitrary
    wordcount = lambda word: sum(ord(letter) - 64 for letter in word)
    return sum(1 for w in words if wordcount(w) in triangles)

"""
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
"""

"""
1,460,357,289 + 4,130,952,867 + 4,106,357,289 + 1,430,952,867 +
1,406,357,289 + 4,160,357,289 = 16,695,334,890
"""

def problem43():
    def perms(a):
        for p in permutations(a):
            p.insert(5,5)
            yield concat2(p);
    def test(n):
        divs = [17,13,11,7,5,3,2];
        for i, div in enumerate(divs):
            if (n // 10**i % 1000) % div != 0:
                return False
        return True
    def testSeries(genx):
        for a in genx:
            if test(a):
                yield a
    ps = perms([0,1,2,3,4,6,7,8,9])
    ret = sum(testSeries(ps))
    return ret

"""
#44: Pentagon numbers

Pentagonal numbers are generated by the formula,
Pn=n(3n-1)/2. The first ten pentagonal numbers are:

1, 5, 12, 22, 35, 51, 70, 92, 117, 145, ...

It can be seen that P4 + P7 = 22 + 70 = 92 = P8. However,
their difference, 70 - 22 = 48, is not pentagonal.

Find the pair of pentagonal numbers, Pj and Pk, for which
their sum and difference are pentagonal and D = |Pk - Pj|
is minimised; what is the value of D?

Antwoord: 5,482,660
"""

def problem44(window = 10**7):
    gen = polygonizer(window, 3)
    next(gen)   # skip 0
    lpgs = list(gen)
    spgs = set(lpgs)
    for a in range(len(lpgs)):
        for b in range(a, len(lpgs)):
            if lpgs[a] + lpgs[b] in spgs and lpgs[b] - lpgs[a] in spgs:
                return lpgs[b] - lpgs[a]
    raise Exception("Answer not found")
    return 0

"""
#45: Triangular, pentagonal, and hexagonal

Triangle, pentagonal, and hexagonal numbers are generated by the following formulae:
Triangle     Tn=n(n+1)/2     1, 3, 6, 10, 15, ...
Pentagonal     Pn=n(3n-1)/2     1, 5, 12, 22, 35, ...
Hexagonal     Hn=n(2n-1)     1, 6, 15, 28, 45, ...

It can be verified that T285 = P165 = H143 = 40755.

Find the next triangle number that is also pentagonal and hexagonal.

Antwoord: 1,533,776,805
"""

"""
T55,385 = P31,977 = H27,693 = 1,533,776,805

every hexagonal number is triangular

triangular root = (sqrt(8n+1)-1)/2
pentagon root = (sqrt(24n+1)+1)/6
hexagon root = (sqrt(8n+1)+1)/4

"""

def problem45():
    spentagon = set(polygonizer(2*10**9, 3))
    for h in [hexagon(n) for n in range(144, 99999)]:
        if h in spentagon: return h
    raise Exception("Answer not found")
    return 0

"""
#46: Goldbach's other conjecture

It was proposed by Christian Goldbach that every odd composite
number can be written as the sum of a prime and twice a square.

9 = 7 + 2x1^2
15 = 7 + 2x2^2
21 = 3 + 2x3^2
25 = 7 + 2x3^2
27 = 19 + 2x2^2
33 = 31 + 2x1^2

It turns out that the conjecture was false.

What is the smallest odd composite that cannot be
written as the sum of a prime and twice a square?

Antwoord: 5,777
"""

def problem46():
    lprimes = list(sieve(10**6))
    ssquares = {2*n*n for n in range(100)}
    def pair(primes, squares, n):
        for prime in primes:
            if prime > n: break
            if n - prime in squares: return (prime, n - prime)
        return (0, 0)
    for i in range(3, 9**9, 2): # 9**9 is arbitrary long number
        if binSearch(lprimes, i): continue
        pr = pair(lprimes, ssquares, i)
        if pr == (0, 0): return i
    raise Exception("Answer not found")
    return 0

"""
#47: Distinct primes factors

The first two consecutive numbers to have two distinct prime factors are:

14 = 2 x 7
15 = 3 x 5

The first three consecutive numbers to have three distinct prime factors are:

644 = 2^2 x 7 x 23
645 = 3 x 5 x 43
646 = 2 x 17 x 19.

Find the first four consecutive integers to have four distinct
prime factors each. What is the first of these numbers?

Antwoord: 134,043
"""

"""
https://blog.dreamshire.com/project-euler-47/
"""

def problem47(L = 3*10**5, nf = 4, ns = 4):
    f = [0]*L
    for n in range(2, L - ns):
        if f[n] == nf:
            c+= 1
            if c == ns: return n-ns+1
        else:
            c = 0
            if f[n] == 0:
                for i in range(n, len(f), n): f[i] += 1

def opdracht47b(distinct = 4, window = 10**6):
    def distinctLen(g): return len(set(g))
    primes = list(sieve(window))
    chain = 0
    for i in range(2, window):
        chain = chain + 1 if distinctLen(primefactors(primes, i)) == distinct else 0
        if chain == distinct:
            return i - (distinct - 1)
    raise Exception("Answer not found")
    return 0

"""
#48: Self powers

The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.

Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.

Antwoord: 9,110,846,700
"""

def problem48(r = range(1,1001)):
    return sum(x**x for x in r) % 10**10

"""
#49: Prime permutations

The arithmetic sequence, 1487, 4817, 8147, in which each of the terms
increases by 3330, is unusual in two ways: (i) each of the three terms
are prime, and, (ii) each of the 4-digit numbers are permutations of one another.

There are no arithmetic sequences made up of three 1-, 2-, or 3-digit
primes, exhibiting this property, but there is one other 4-digit
increasing sequence.

What 12-digit number do you form by concatenating the three terms in this sequence?

Antwoord: 296,962,999,629
"""

def problem49():
    sp4 = {x for x in sieve(10**4) if x > 1487}
    for p in sp4:
        if p+3330 in sp4 and p+6660 in sp4 and sameDigs(p,p+3330) and sameDigs(p,p+6660):
            return (p + 6660) + (p + 3330) * 10**4 + p * 10**8
    raise Exception("Answer not found")
    return 0

"""
#50: Consecutive prime sum

The prime 41, can be written as the sum of six consecutive primes:
41 = 2 + 3 + 5 + 7 + 11 + 13

This is the longest sum of consecutive primes that adds to a prime below one-hundred.

The longest sum of consecutive primes below one-thousand
that adds to a prime, contains 21 terms, and is equal to 953.

Which prime, below one-million, can be written as the sum of the most consecutive primes?

Antwoord: 997,651
"""

def problem50(limit = 10**6):
    best_prime, best_sum = 0, 0
    lprimes = list(sieve(limit))
    xlen = len(lprimes)
    for i in range(xlen):
        for j in range(i + best_sum, xlen):
            xsum = sum(lprimes[i:j + 1])
            if xsum >= limit: break
            sublen = (j + 1) - i;
            if binSearch(lprimes, xsum) and sublen > best_sum:
                best_sum, best_prime = sublen, xsum
    return best_prime;

"""
#51: Prime digit replacements

By replacing the 1st digit of the 2-digit number *3, it turns out that six
of the nine possible values: 13, 23, 43, 53, 73, and 83, are all prime.

By replacing the 3rd and 4th digits of 56**3 with the same digit, this
5-digit number is the first example having seven primes among the ten
generated numbers, yielding the family: 56003, 56113, 56333, 56443,
56663, 56773, and 56993. Consequently 56003, being the first member of
this family, is the smallest prime with this property.

Find the smallest prime which, by replacing part of the number (not
necessarily adjacent digits) with the same digit, is part of an eight
prime value family.

Antwoord: 121,313
"""

def problem51():
    def binarize(n):
        div, dec = 2, 1
        while n:
            yield dec if n % div else 0
            n -= n % div
            div *= 2
            dec *= 10
    lprimes = list(sieve(10**6))
    sprimes = set(lprimes)
    def family(primes, n, mask):
        xlen = decimals(n)
        bmask = list(binarize(mask))
        for i, b in enumerate(bmask): n -= b * digit(n, i)
        for a in range(0, 10):
            tmp = n
            for b in bmask: tmp += b * a
            if decimals(tmp) == xlen and tmp in primes: yield tmp
    for i in lprimes:
        for mask in range(1, 2**decimals(i)):
            if len(list(family(sprimes, i, mask))) == 8:
                return min(family(sprimes, i, mask))
    return 0

"""
#52: Pandigital multiples

It can be seen that the number, 125874, and its double, 251748,
contain exactly the same digits, but in a different order.

Find the smallest positive integer, x, such that
2x, 3x, 4x, 5x, and 6x, contain the same digits.

Antwoord: 142,857
"""

"""
https://blog.dreamshire.com/project-euler-52-solution/
"""
def problem52():
    f = lambda n:sorted(digits(n))
    n = 99999
    while not f(n*2) == f(n*3) == f(n*4) == f(n*5) == f(n*6): n += 9
    return n

"""
#53: Combinatoric selections

There are exactly ten ways of selecting three from five, 12345:

123, 124, 125, 134, 135, 145, 234, 235, 245, and 345

In combinatorics, we use the notation, 5C3 = 10.

In general,
nCr =  
n!
r!(n-r)!
 ,where r <= n, n! = nx(n-1)x...x3x2x1, and 0! = 1.

It is not until n = 23, that a value exceeds one-million: 23C10 = 1144066.

How many, not necessarily distinct, values of  nCr,
for 1 <= n <= 100, are greater than one-million?

Antwoord: 4,075
"""

def problem53(nlimit = 101, limit = 10**6):
    tree = []
    for i in range(nlimit):
        tree.append([0] * nlimit)
    tree[0][0] = 1
    for i in range(nlimit - 1):
        tree[i+1][0] = 1
        for j in range(i + 1):
            tree[i+1][j + 1] = min(sum(tree[i][j:j+2]), limit+1)
    ncount = 0
    for row in tree:
        for n in row:
            if n > limit:
                ncount += 1
    return ncount

def opdracht53b():
    xcount = 0
    for a in range(23,101):
        for b in range(4, a - 3):
            if combinations(a, b) >= 10**6:
                xcount += a - b * 2 + 1
                break
    return xcount

"""
#54 Poker hands

In the card game poker, a hand consists of five cards and
are ranked, from lowest to highest, in the following way:

High Card: Highest value card.
One Pair: Two cards of the same value.
Two Pairs: Two different pairs.
Three of a Kind: Three cards of the same value.
Straight: All cards are consecutive values.
Flush: All cards of the same suit.
Full House: Three of a kind and a pair.
Four of a Kind: Four cards of the same value.
Straight Flush: All cards are consecutive values of same suit.
Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.

The cards are valued in the order:
2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.

If two players have the same ranked hands then the rank made up of the
highest value wins; for example, a pair of eights beats a pair of fives
(see example 1 below). But if two ranks tie, for example, both players
have a pair of queens, then highest cards in each hand are compared (see
example 4 below); if the highest cards tie then the next highest cards
are compared, and so on.

Consider the following five hands dealt to two players:

Hand    Player 1            Player 2          Winner

 1    5H 5C 6S 7S KD     2C 3S 8S 8D TD      Player 2
      Pair of Fives      Pair of Eights

 2   5D 8C 9S JS AC      2C 5C 7D 8S QH      Player 1
    Highest card Ace   Highest card Queen

 3   2D 9C AS AH AC      3D 6D 7D TD QD      Player 2
       Three Aces     Flush with Diamonds

 4   4D 6S 9H QH QC      3D 6D 7H QD QS      Player 1
     Pair of Queens      Pair of Queens
    Highest card Nine  Highest card Seven
   
 5   2H 2D 4C 4D 4S     3C 3D 3S 9S 9D       Player 1
       Full House         Full House
   With Three Fours   with Three Threes

The file, poker.txt, contains one-thousand random hands dealt to two
players. Each line of the file contains ten cards (separated by a single
space): the first five are Player 1's cards and the last five are Player 2's
cards. You can assume that all hands are valid (no invalid characters or
repeated cards), each player's hand is in no specific order, and in each
hand there is a clear winner.

How many hands does Player 1 win?

Antwoord: 376
"""

"""
https://blog.dreamshire.com/project-euler-54-solution/
"""

def problem54():
    hands = (line.split() for line in open("euler54.txt"))
    values = {r:i for i,r in enumerate('23456789TJQKA', 2)}
    straights = [(v, v-1, v-2, v-3, v-4) for v in range(14, 5, -1)] + [(14, 5, 4, 3, 2)]
    ranks = [(1,1,1,1,1),(2,1,1,1),(2,2,1),(3,1,1),(),(),(3,2),(4,1)]
    def hand_rank(hand):
        values2 = (x[0] for x in hand)
        cnt2 = kounter(values2).items()
        score = list(zip(*sorted(((v, values[k]) for k,v in cnt2), reverse=True)))
        score[0] = ranks.index(score[0])
        if len(set(card[1] for card in hand)) == 1:
            score[0] = 5  # flush
        if score[1] in straights:
            score[0] = 8 if score[0] == 5 else 4  # str./str. flush
        return score
    player1 = 0
    for hand in hands:
        if hand_rank(hand[:5]) > hand_rank(hand[5:]):
            player1 += 1
    return player1

"""
#55: Lychrel numbers

If we take 47, reverse and add, 47 + 74 = 121, which is palindromic.

Not all numbers produce palindromes so quickly. For example,

349 + 943 = 1292,
1292 + 2921 = 4213
4213 + 3124 = 7337

That is, 349 took three iterations to arrive at a palindrome.

Although no one has proved it yet, it is thought that some numbers, like
196, never produce a palindrome. A number that never forms a palindrome
through the reverse and add process is called a Lychrel number. Due to the
theoretical nature of these numbers, and for the purpose of this problem,
we shall assume that a number is Lychrel until proven otherwise. In
addition you are given that for every number below ten-thousand, it will
either (i) become a palindrome in less than fifty iterations, or, (ii) no
one, with all the computing power that exists, has managed so far to map it
to a palindrome. In fact, 10677 is the first number to be shown to require
over fifty iterations before producing a palindrome:
4668731596684224866951378664 (53 iterations, 28-digits).

Surprisingly, there are palindromic numbers that are
themselves Lychrel numbers; the first example is 4994.

How many Lychrel numbers are there below ten-thousand?

NOTE: Wording was modified slightly on 24 April 2007 to
emphasise the theoretical nature of Lychrel numbers.

Antwoord: 249
"""

def islychrel(n, it = 50):
    for i in range(it):
        n += reverse(n)
        if ispalindrome(n): return False
    return True

def problem55(r = range(10000)):
    return sum(islychrel(n) for n in r)

"""
#56: Powerful digit sum

A googol (10^100) is a massive number: one followed by one-hundred zeros;
100^100 is almost unimaginably large: one followed by two-hundred zeros.
Despite their size, the sum of the digits in each number is only 1.

Considering natural numbers of the form, ab,
where a, b < 100, what is the maximum digital sum?

Antwoord: 972
"""

def problem56():
    best = 0
    for a in range(100):
        for b in range(100):
            best = max(best, sum(digits(a**b)))
    return best

"""
#57: Square root convergents

It is possible to show that the square root of two
can be expressed as an infinite continued fraction.

sqrt 2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...

By expanding this for the first four iterations, we get:

1 + 1/2 = 3/2 = 1.5
1 + 1/(2 + 1/2) = 7/5 = 1.4
1 + 1/(2 + 1/(2 + 1/2)) = 17/12 = 1.41666...
1 + 1/(2 + 1/(2 + 1/(2 + 1/2))) = 41/29 = 1.41379...

The next three expansions are 99/70, 239/169, and 577/408, but the eighth
expansion, 1393/985, is the first example where the number of digits
in the numerator exceeds the number of digits in the denominator.

In the first one-thousand expansions, how many fractions
contain a numerator with more digits than denominator?

Antwoord: 153
"""

"""
https://www.xarg.org/puzzle/project-euler/problem-57/
"""

def problem57(N=1000):
    c = 0
    n = d = 1
    np = dp = 10
    for k in range(N):
        n, d = 2 * d + n, d + n
        if n >= np: np*= 10
        if d >= dp: dp*= 10
        if np > dp: c+= 1
    return c

"""
#58: Spiral primes

Starting with 1 and spiralling anticlockwise in the following
way, a square spiral with side length 7 is formed.

37 36 35 34 33 32 31
38 17 16 15 14 13 30
39 18  5  4  3 12 29
40 19  6  1  2 11 28
41 20  7  8  9 10 27
42 21 22 23 24 25 26
43 44 45 46 47 48 49

It is interesting to note that the odd squares lie along the bottom right
diagonal, but what is more interesting is that 8 out of the 13 numbers
lying along both diagonals are prime; that is, a ratio of 8/13 ? 62%.

If one complete new layer is wrapped around the spiral above, a square
spiral with side length 9 will be formed. If this process is continued,
what is the side length of the square spiral for which the ratio of primes
along both diagonals first falls below 10%?

Antwoord: 26,241
"""

"""
https://blog.dreamshire.com/project-euler-58-solution/
"""

def problem58():
    n_prime, d, avg, n = 0, 1, 1, 2
    while avg >= 0.10:
        n_prime += miller_rabin(d + n) + miller_rabin(d + n*2) + miller_rabin(d + n*3)
        d += n*4
        n += 2
        avg = n_prime / (2.0 * n)
    return n - 1

"""
#59: XOR decryption

Each character on a computer is assigned a unique code and the preferred
standard is ASCII (American Standard Code for Information Interchange).
For example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.

A modern encryption method is to take a text file, convert the bytes to
ASCII, then XOR each byte with a given value, taken from a secret key. The
advantage with the XOR function is that using the same encryption key on
the cipher text, restores the plain text; for example, 65 XOR 42 = 107,
then 107 XOR 42 = 65.

For unbreakable encryption, the key is the same length as the plain text
message, and the key is made up of random bytes. The user would keep the
encrypted message and the encryption key in different locations, and
without both "halves", it is impossible to decrypt the message.

Unfortunately, this method is impractical for most users, so the modified
method is to use a password as a key. If the password is shorter than the
message, which is likely, the key is repeated cyclically throughout the
message. The balance for this method is using a sufficiently long password
key for security, but short enough to be memorable.

Your task has been made easy, as the encryption key consists of three lower
case characters. Using cipher.txt (right click and 'Save Link/Target As...'),
a file containing the encrypted ASCII codes, and the knowledge that the plain
text must contain common English words, decrypt the message and find the sum
of the ASCII values in the original text.

Antwoord: 107,359
"""

"""
key = "god"
"""

def decipher(msg, key):
    for i, c in enumerate(msg):
        yield chr(c ^ key[i % 3])

def keygen(xmin, xmax):
    for i in range(xmin,xmax):
        for j in range(xmin,xmax):
            for k in range(xmin, xmax):
                yield (i, j, k)

english = {'a': 0.08167, 'b': 0.01492, 'c': 0.02782, 'd': 0.04253, 'e': 0.12702,
    'f': 0.02228, 'g': 0.02015, 'h': 0.06094, 'i': 0.06966, 'j': 0.0153, 'k': 0.0772,
    'l': 0.04025, 'm': 0.02406, 'n': 0.06749, 'o': 0.07507, 'p': 0.01929, 'q': 0.00095,
    'r': 0.05987, 's': 0.06327, 't': 0.09056, 'u': 0.02758, 'v': 0.00978, 'w': 0.02360,
    'x': 0.00150, 'y': 0.01974, 'z': 0.00074}

def analysis(s):
    absolute = dict()
    letters = 0
    for c in s:
        if c.isalpha():
            letters += 1
            low = c.lower()
            if low in absolute:
                absolute[low] += 1
            else:
                absolute[low] = 1
    relative = {key : value / letters for key, value in absolute.items()}
    return letters, absolute, relative

def compare(a, b):
    sumdifference = 0
    for key, value in a.items():
        sumdifference += abs(value - b[key])
    return sumdifference

def problem59(fn = "euler59.txt"):
    msg = [int(n) for n in open(fn).read().split()]
    best_sumdif = 999999.9
    best_key = (0, 0, 0)
    for key in keygen(97, 123):
        s = "".join(decipher(msg, key))
        letters, absolute, relative = analysis(s)
        sumdif = compare(relative, english)
        if sumdif < best_sumdif:
            best_sumdif = sumdif
            best_key = key
    s = "".join(decipher(msg, best_key))
    return sum(ord(c) for c in s)

"""
#60: Prime pair sets

The primes 3, 7, 109, and 673, are quite remarkable. By taking any two
primes and concatenating them in any order the result will always be
prime. For example, taking 7 and 109, both 7109 and 1097 are prime. The
sum of these four primes, 792, represents the lowest sum for a set of
four primes with this property.

Find the lowest sum for a set of five primes for which any two primes
concatenate to produce another prime.

Antwoord: 26,033
"""

"""
13 + 5,197 + 5,701 + 6,733 + 8,389 = 26,033
"""

"""
http://radiusofcircle.blogspot.com/2016/10/problem-60-project-euler-solution-with.html
"""

def problem60():
    comb = lambda a, b: miller_rabin(a*10**decimals(b)+b) and miller_rabin(b*10**decimals(a)+a)
    primes = list(sieve(10000))
    for a in primes:
        for b in primes:
            if b < a: continue
            if comb(a, b):
                for c in primes:
                    if c < b: continue
                    if comb(a, c) and comb(b, c):
                        for d in primes:
                            if d < c: continue
                            if comb(a, d) and comb(b, d) and comb(c, d):
                                for e in primes:
                                    if e < d: continue
                                    if comb(a, e) and comb(b, e) and comb(c, e) and comb(d, e):
                                        return a+b+c+d+e

"""
#61: Cyclical figurate numbers

Triangle, square, pentagonal, hexagonal, heptagonal, and octagonal numbers
are all figurate (polygonal) numbers and are generated by the following formulae:
Triangle     P3,n=n(n+1)/2     1, 3, 6, 10, 15, ...
Square     P4,n=n2     1, 4, 9, 16, 25, ...
Pentagonal     P5,n=n(3n-1)/2     1, 5, 12, 22, 35, ...
Hexagonal     P6,n=n(2n-1)     1, 6, 15, 28, 45, ...
Heptagonal     P7,n=n(5n-3)/2     1, 7, 18, 34, 55, ...
Octagonal     P8,n=n(3n-2)     1, 8, 21, 40, 65, ...

The ordered set of three 4-digit numbers: 8128, 2882, 8281, has three interesting properties.

1.    The set is cyclic, in that the last two digits of each number is the
first two digits of the next number (including the last number with the first).
2.    Each polygonal type: triangle (P3,127=8128), square (P4,91=8281), and
pentagonal (P5,44=2882), is represented by a different number in the set.
3.    This is the only set of 4-digit numbers with this property.

Find the sum of the only ordered set of six cyclic 4-digit numbers for which
each polygonal type: triangle, square, pentagonal, hexagonal, heptagonal,
and octagonal, is represented by a different number in the set.

Antwoord: 28,684
"""

"""
8128 (Hex) + 2882 (Pent) + 8256 (Tri) + 5625 (Sq) + 2512 (Hept) + 1281 (Oct) = 28,684
"""

def problem61():
    func=[triangle,square,pentagon,hexagon,heptagon,octagon]
    A=list()
    for f in range(0,6):
        A += [set()]
        for n in range(15,142):
            val=func[f](n)
            if 1000 <= val <=9999:A[f].add(int(val))
    for p in permutations2(range(6)):
        for n0 in A[p[0]]:
            for n1 in A[p[1]]:
                if n1//100==n0%100:
                    for n2 in A[p[2]]:
                        if n2//100==n1%100:
                            for n3 in A[p[3]]:
                                if n3//100==n2%100:
                                    for n4 in A[p[4]]:
                                        if n4//100==n3%100:
                                            for n5 in A[p[5]]:
                                                if n5//100==n4%100:
                                                    if n5%100 ==n0//100:
                                                        return sum((n0,n1,n2,n3,n4,n5))

"""
#62: Cubic permutations

The cube, 41063625 (345^3), can be permuted to produce two other
cubes: 56623104 (384^3) and 66430125 (405^3). In fact, 41063625 is
the smallest cube which has exactly three permutations of its
digits which are also cube.

Find the smallest cube for which exactly
five permutations of its digits are cube.

Antwoord: 127,035,954,683
"""

def problem62():
    lst = list(n**3 for n in range(9000))
    for i, a in enumerate(lst):
        cnt = 0
        ln = decimals(a)
        for b in range(i, len(lst)):
            if decimals(lst[b]) > ln: break
            if sameDigs(a, lst[b]): cnt += 1
        if cnt == 5:
            return a
    raise Exception("Answer not found")
    return 0

"""
#63: Powerful digit counts

The 5-digit number, 16807=7^5, is also a fifth power. Similarly,
the 9-digit number, 134217728=8^9, is a ninth power.

How many n-digit positive integers exist which are also an nth power?

Antwoord: 49
"""

"""
1^1 ~ 9^1, 4^2 ~ 9^2, 5^3 ~ 9^3, 6^4 ~ 9^4, 7^5 ~ 9^5, 7^6 ~ 9^6,
8^7, 9^7, 8^8, 9^8, 8^9, 9^9, 8^10, 9^10, 9^11, 9^12, 9^13, 9^14,
9^15, 9^16, 9^17, 9^18, 9^19, 9^20, 9^21
"""

def problem63():
    xsum = 0
    for e in range(1,999999):
        subsum = 0
        for base in range(1,10):
            subsum += decimals(base**e) == e
        xsum += subsum
        if subsum == 0: break
    return xsum

"""
#64: Odd period square roots

Antwoord: 1,322
"""

"""
https://blog.dreamshire.com/project-euler-64-solution/
"""

def problem64():
    L, odd_period = 10000, 0
    for N in range(2, L + 1):
        r = limit = floorsqrt(N)
        if limit ** 2 == N: continue
        k, period = 1, 0
        while k != 1 or period == 0:
            k = (N - r * r) // k
            r = (limit + r) // k * k - r
            period += 1
        if period % 2 == 1: odd_period += 1
    return odd_period

"""
#65: Convergents of e

Antwoord: 272
"""

"""
https://blog.dreamshire.com/project-euler-65-solution/
"""

def problem65():
    n0, n1, L = 1, 2, 100
    for i in range(2, L+1): 
        n0, n1 = n1, n0 + n1 * (1 if i%3 else 2 * i//3)
    return sum(map(int, str(n1)))

"""
#66: Diophantine equation

Antwoord: 661
"""

"""
adapted from: https://euler.stephan-brumme.com/66/
"""

def problem66():
    best_x, best_d = 0, 0
    for d in range(2, 1001):
        root = floorsqrt(d)
        if root * root == d:
            continue
        a, numerator, denominator = root, 0, 1
        x = [0, 1, root]
        y = [0, 0, 1]
        while True:
            numerator = denominator * a - numerator
            denominator = (d - numerator * numerator) // denominator
            a = (root + numerator) // denominator
            x[0] = x[1]
            x[1] = x[2]
            x[2] = x[1] * a + x[0]
            y[0] = y[1]
            y[1] = y[2]
            y[2] = y[1] * a + y[0]
            if x[2]**2 == y[2]**2 * d + 1:
                break
        if best_x < x[2]:
            best_x, best_d = x[2], d
    return best_d

"""
#67: Maximum path sum II

By starting at the top of the triangle below and moving to adjacent
numbers on the row below, the maximum total from top to bottom is 23.

3
7 4
2 4 6
8 5 9 3

That is, 3 + 7 + 4 + 9 = 23.

Find the maximum total from top to bottom in triangle.txt (right click
and 'Save Link/Target As...'), a 15K text file containing a triangle with
one-hundred rows.

NOTE: This is a much more difficult version of Problem 18. It is not
possible to try every route to solve this problem, as there are 299
altogether! If you could check one trillion (1012) routes every second
it would take over twenty billion years to check them all. There is an
efficient algorithm to solve it. ;o)

Antwoord: 7,273
"""

""" adapted from opdracht18 """
def problem67(fn = "euler67.txt", root = 100):
    triangle2 = [int(n) for n in open(fn).read().split()]
    while root > 1:
        for i in range(root - 1):
            j = triangle(root - 2) + i
            k = triangle(root - 1) + i
            triangle2[j] += max(triangle2[k], triangle2[k + 1])
        root -= 1
    return triangle2[0]

"""
#68: Magic 5-gon ring

Consider the following "magic" 3-gon ring, filled
with the numbers 1 to 6, and each line adding to nine.

Working clockwise, and starting from the group of three with the
numerically lowest external node (4,3,2 in this example), each
solution can be described uniquely. For example, the above solution
can be described by the set: 4,3,2; 6,2,1; 5,1,3.

It is possible to complete the ring with four different
totals: 9, 10, 11, and 12. There are eight solutions in total.

Total    Solution Set
  9   4,2,3; 5,3,1; 6,1,2
  9   4,3,2; 6,2,1; 5,1,3
  10  2,3,5; 4,5,1; 6,1,3
  10  2,5,3; 6,3,1; 4,1,5
  11  1,4,6; 3,6,2; 5,2,4
  11  1,6,4; 5,4,2; 3,2,6
  12  1,5,6; 2,6,4; 3,4,5
  12  1,6,5; 3,5,4; 2,4,6

By concatenating each group it is possible to form 9-digit
strings; the maximum string for a 3-gon ring is 432621513.

Using the numbers 1 to 10, and depending on arrangements, it is
possible to form 16- and 17-digit strings. What is the maximum
16-digit string for a "magic" 5-gon ring?

Antwoord: 6,531,031,914,842,725
"""

def ngon3():
    for p in permutations2(range(1,7)):
        if p[0] + p[3] + p[4] != p[1] + p[4] + p[5]: continue
        if p[0] + p[3] + p[4] != p[2] + p[5] + p[3]: continue
        if p[0] > p[1] or p[0] > p[2]: continue
        print("{}  {},{},{}; {},{},{}; {},{},{}".format(
            p[0] + p[3] + p[4], p[0], p[3], p[4], p[1], p[4], p[5], p[2], p[5], p[3]))

def sumEqual(lst):
    for i in range(1,5):
        if sum(lst[0]) != sum(lst[i]):
            return False
    return True

def ngon5():
    for p in permutations2(range(1,11)):
        lines = list();
        lines.append((p[0], p[5], p[6]))
        lines.append((p[1], p[6], p[7]))
        lines.append((p[2], p[7], p[8]))
        lines.append((p[3], p[8], p[9]))
        lines.append((p[4], p[9], p[5]))
        if sumEqual(lines) == False: continue
        if p[0] > p[1] or p[0] > p[2] or p[0] > p[3] or p[0] > p[4]: continue
        xstr = str()
        for line in lines:
            for n in line:
                xstr += str(n)
        yield int(xstr)

def problem68():
    return last(ngon5())

"""
#69: Totient maximum

Euler's Totient function, phi(n) [sometimes called the phi function], is
used to determine the number of numbers less than n which are relatively
prime to n. For example, as 1, 2, 4, 5, 7, and 8, are all less than nine
and relatively prime to nine, phi(9)=6.

Antwoord: 510,510
"""

"""
https://blog.dreamshire.com/project-euler-69-solution/
"""

def problem69(L = 10**6):
    lprimes = list(sieve(100))
    maxn = 1
    for p in lprimes:
        if maxn*p > L: return maxn
        maxn *= p
    raise "Buy me some more prime numbers!"
    return 0

"""
#70: Totient permutation

Euler's Totient function, phi(n) [sometimes called the phi function],
is used to determine the number of positive numbers less than or equal to
n which are relatively prime to n. For example, as 1, 2, 4, 5, 7, and 8,
are all less than nine and relatively prime to nine, phi(9)=6.
The number 1 is considered to be relatively prime to every positive
number, so phi(1)=1.

Interestingly, phi(87109)=79180, and it can be
seen that 87109 is a permutation of 79180.

Find the value of n, 1 < n < 107, for which phi(n) is a
permutation of n and the ratio n/phi(n) produces a minimum.

Answer: 8,319,823
"""

"""
https://blog.dreamshire.com/project-euler-70-solution/
"""

def problem70(L = 10**7):
    primes = list(sieve(int(floorsqrt(L) * 1.2)))
    min_q, min_n, i = 2, 0, 0
    for p1 in primes:
        i+=1
        for p2 in primes[i:]:
            if (p1+p2)%9 != 1: continue
            n = p1 * p2
            if n > L: return min_n
            phi = (p1-1) * (p2-1)
            q = n / float(phi)
            if sameDigs(phi, n) and min_q>q: min_q, min_n = q, n
    raise("Answer not found")
    return 0

"""
#71: Ordered fractions

Consider the fraction, n/d, where n and d are positive integers.
If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions
for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2,
4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that 2/5 is the fraction immediately to the left of 3/7.

By listing the set of reduced proper fractions for d <= 1,000,000 in
ascending order of size, find the numerator of the fraction immediately
to the left of 3/7.

Antwoord: 428,570
"""

"""
Adapted from:
https://www.mathblog.dk/project-euler-71-proper-fractions-ascending-order/
"""

def problem71(limit = 10**6):
    a, b, r, s = 3,7,0,1
    q = limit
    while q > 2:
        p = (a * q - 1) // b
        if p * s > r * q:
            s, r = q, p
        q -= 1
    return r

"""
#72: Counting fractions

Antwoord: 303,963,552,391
"""

def problem72(L = 10**6):
    phi = list(range(L + 1))
    for n in range(2, L + 1):
        if phi[n] == n:
            for k in range(n, L + 1, n):
                phi[k] -= phi[k] // n
    return sum(phi) - 1

"""
#73: Counting fractions in a range

Consider the fraction, n/d, where n and d are positive integers.
If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions
for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7,
1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 3 fractions between 1/3 and 1/2.

How many fractions lie between 1/3 and 1/2 in the
sorted set of reduced proper fractions for d <= 12,000?

Antwoord: 7,295,372
"""

"""
https://blog.dreamshire.com/project-euler-73-solution/
"""

def problem73(L = 12000, n1 = 1, d1 = 3, n2 = 1, d2 = 2):
    ceildiv = lambda a, b: -(-a // b)
    n = [0] * (L+1) 
    for d in range(1, L+1):
        n[d] += ceildiv(n2*d, d2) - ceildiv(n1*d, d1) - 1
        n[2*d::d] = [k-n[d] for k in n[2*d::d]]
    return sum(n)

"""
#74: Digit factorial chains

The number 145 is well known for the property that the
sum of the factorial of its digits is equal to 145:

1! + 4! + 5! = 1 + 24 + 120 = 145

Perhaps less well known is 169, in that it produces the longest chain of
numbers that link back to 169; it turns out that there are only three such
loops that exist:

169 -> 363601 -> 1454 -> 169
871 -> 45361 -> 871
872 -> 45362 -> 872

It is not difficult to prove that EVERY starting number
will eventually get stuck in a loop. For example,

69 -> 363600 -> 1454 -> 169 -> 363601 (-> 1454)
78 -> 45360 -> 871 -> 45361 (-> 871)
540 -> 145 (-> 145)

Starting with 69 produces a chain of five non-repeating terms, but the longest
non-repeating chain with a starting number below one million is sixty terms.

How many chains, with a starting number below one
million, contain exactly sixty non-repeating terms?

Antwoord: 402
"""

def dfchain(n):
    previous = set()
    while True:
        yield n
        if n in previous: return
        previous.add(n)
        n = sum(factorial(digit) for digit in digits(n))

def dfccnt(cache, n):
    previous = set()
    count = 0
    while True:
        if n in cache: return count + cache[n]
        count += 1
        if n in previous: return count
        previous.add(n)
        n = sum(factorial(digit) for digit in digits(n))

def problem74():
    count = 0
    cache = dict()
    for n in range(3,10**6):
        cache[n] = dfccnt(cache, n)
    return sum(n == 61 for n in cache.values())

"""
#75: Singular integer right triangles

It turns out that 12 cm is the smallest length of wire that can be bent
to form an integer sided right angle triangle in exactly one way, but
there are many more examples.

12 cm: (3,4,5)
24 cm: (6,8,10)
30 cm: (5,12,13)
36 cm: (9,12,15)
40 cm: (8,15,17)
48 cm: (12,16,20)

In contrast, some lengths of wire, like 20 cm, cannot be bent to form an
integer sided right angle triangle, and other lengths allow more than one
solution to be found; for example, using 120 cm it is possible to form
exactly three different integer sided right angle triangles.

120 cm: (30,40,50), (20,48,52), (24,45,51)

Given that L is the length of the wire, for how many values of
L <= 1,500,000 can exactly one integer sided right angle triangle be formed?

Antwoord: 161,667
"""

"""
https://blog.dreamshire.com/project-euler-75-solution/
"""

def problem75():
    L = 15*10**5+1
    maybe = set()
    nope = set()
    for m in range(2, floorsqrt(L//2)):
        for n in range(m - 1, 0, -2):
            if gcd(m, n) == 1:
                s = 2 * (m * m + m * n)
                for k in range(1, L//s + 1):
                    nope.add(k*s) if k*s in maybe else maybe.add(k*s)
    return len(maybe-nope)

"""
#76: Counting summations

It is possible to write five as a sum in exactly six different ways:

4 + 1
3 + 2
3 + 1 + 1
2 + 2 + 1
2 + 1 + 1 + 1
1 + 1 + 1 + 1 + 1

How many different ways can one hundred be written
as a sum of at least two positive integers?

Antwoord: 190,569,291
"""

""" adapted from opdracht31 """
def problem76(target = 100, coins = list(range(100))):
    return ways(target, coins)

"""
#77: Prime summations

It is possible to write ten as the sum of primes in exactly five different ways:

7 + 3
5 + 5
5 + 3 + 2
3 + 3 + 2 + 2
2 + 2 + 2 + 2 + 2

What is the first value which can be written as the
sum of primes in over five thousand different ways?

Antwoord: 71
"""

def problem77():
    lprimes = list(sieve(100))
    for n in range(10, 100):
        if ways(n, lprimes) > 5000:
            return n
    raise("Answer not found")
    return 0

"""
#78: Coin partitions

Let p(n) represent the number of different ways in which n coins can be
separated into piles. For example, five coins can be separated into piles
in exactly seven different ways, so p(5)=7.

  OOOOO
 OOOO O
 OOO OO
 OOO O O
 OO OO O
OO O O O
O O O O O

Find the least value of n for which p(n) is divisible by one million.

Antwoord: 55,374
"""

"""
https://blog.dreamshire.com/project-euler-78-solution/
"""
def problem78():
    k = sum([[i*(3*i - 1)//2, i*(3*i - 1)//2 + i] for i in range(1, 250)], [])
    p, sgn, n, m  = [1], [1, 1, -1, -1], 0, 1e6
    while p[n]>0:    # expand generating function to calculate p(n)
        n += 1
        px, i = 0, 0
        while k[i] <= n:
            px += p[n - k[i]] * sgn[i%4]
            i += 1
        p.append(px % m)
    return n

"""
#79: Passcode derivation

A common security method used for online banking is to ask the user for
three random characters from a passcode. For example, if the passcode
was 531278, they may ask for the 2nd, 3rd, and 5th characters; the
expected reply would be: 317.

The text file, keylog.txt, contains fifty successful login attempts.

Given that the three characters are always asked for in order, analyse the
file so as to determine the shortest possible secret passcode of unknown length.

Antwoord: 73,162,890
"""

"""
129, 160, 162, 168, 180, 289, 290, 316, 318, 319, 362, 368, 380, 389, 620, 629, 680,
689, 690, 710, 716, 718, 719, 720, 728, 729, 731, 736, 760, 762, 769, 790, 890

73162890
"""

def problem79(fn = "euler79.txt"):
    def test(perm, attempts):
        def login(passcode, attempt):
            arr = list()
            for n in attempt:
                arr.append(passcode.index(n))
            return sorted(arr) == arr
        for attempt in attempts:
            if login(perm, reversed(list(digits(attempt)))) == False: return False
        return True
    attempts = {int(n) for n in open(fn).read().split()}
    digs = set()
    for attempt in attempts:
        for dig in digits(attempt):
            digs.add(dig)
    for perm in permutations2(digs):
        if test(perm, attempts): return concat2(perm)
    raise("Answer not found")
    return 0

"""
#80: Square root digital expansion

It is well known that if the square root of a natural number is not an
integer, then it is irrational. The decimal expansion of such square
roots is infinite without any repeating pattern at all.

The square root of two is 1.41421356237309504880..., and the
digital sum of the first one hundred decimal digits is 475.

For the first one hundred natural numbers, find the total of the digital sums
of the first one hundred decimal digits for all the irrational square roots.

Antwoord: 40,886
"""

"""
dsums = {2: 475, 3: 441, 5: 473, 6: 471, 7: 398, 8: 465, 10: 459, 11: 484, 12: 406,
13: 418, 14: 485, 15: 500, 17: 450, 18: 401, 19: 472, 20: 488, 21: 484,
22: 407, 23: 453, 24: 484, 26: 455, 27: 398, 28: 401, 29: 459, 30: 467,
31: 473, 32: 471, 33: 472, 34: 459, 35: 440, 37: 457, 38: 462, 39: 465,
40: 486, 41: 451, 42: 445, 43: 503, 44: 483, 45: 440, 46: 451, 47: 423,
48: 398, 50: 470, 51: 468, 52: 432, 53: 477, 54: 461, 55: 432, 56: 467,
57: 453, 58: 409, 59: 499, 60: 479, 61: 436, 62: 450, 63: 439, 65: 440,
66: 464, 67: 498, 68: 451, 69: 438, 70: 464, 71: 417, 72: 424, 73: 427,
74: 427, 75: 435, 76: 459, 77: 484, 78: 456, 79: 461, 80: 500, 82: 455,
83: 474, 84: 456, 85: 426, 86: 459, 87: 459, 88: 409, 89: 408, 90: 477,
91: 412, 92: 502, 93: 452, 94: 502, 95: 459, 96: 483, 97: 440, 98: 477, 99: 446}
"""

def squaredigits(square, digits = 100):
    buf = [0] * 100
    for digit in range(digits):
        for n in range(10):
            buf[digit] = 9 - n
            if concat2(buf[:(digit + 1)])**2 < (square * 10**(digit*2)):
                break
    return sum(buf)

def problem80():
    return sum(squaredigits(n, 100) for n in range(2, 100) if issquare(n) == False)

"""
#81: Path sum: two ways

In the 5 by 5 matrix below, the minimal path sum from the top left to the
bottom right, by only moving to the right and down, is indicated in bold
red and is equal to 2427.

131  673  234  103  18
201  96   342  965  150
630  803  746  422  111
537  699  497  121  956
805  732  524  37   331

Find the minimal path sum, in matrix.txt (right click and "Save Link/Target
As..."), a 31K text file containing a 80 by 80 matrix, from the top left to
the bottom right by only moving right and down.

Antwoord: 427,337
"""

def problem81(fn = "euler81.txt"):
    lst = [int(n) for n in open(fn).read().split()]
    root = floorsqrt(len(lst))
    for i in range(root - 1):
        lst[i + 1] += lst[i]
    for i in range(root - 1):
        lst[(i + 1) * root] += lst[i * root]
    for i in range(root - 1):
        for j in range(root - 1):
            lst[(i + 1) * root + (j + 1)] += min(lst[(i + 1)*root+j], lst[i*root + (j+1)])
    return lst[len(lst) - 1]

"""
#82: Path sum: three ways

NOTE: This problem is a more challenging version of Problem 81.

Antwoord: 260,324
"""

def problem82(fn = "euler82.txt"):
    lst = [int(n) for n in open(fn).read().split()]
    grid = [[0 for i in range(80)] for j in range(80)]
    sol = [0] * 80
    for i, n in enumerate(lst):
        grid[i // 80][i % 80] = n
    for i in range(80):
        sol[i] = grid[i][79]
    for i in range(79, 0, -1):
        sol[0] += grid[0][i - 1]
        for j in range(1, 80):
            sol[j] = min(sol[j - 1] + grid[j][i - 1], sol[j] + grid[j][i - 1])
        for j in range(79, 0, -1):
            sol[j - 1] = min(sol[j - 1], sol[j] + grid[j - 1][i - 1])
    return min(sol)

"""
#83: Path sum: four ways

NOTE: This problem is a significantly more challenging version of Problem 81.

In the 5 by 5 matrix below, the minimal path sum from the top left to the
bottom right, by moving left, right, up, and down, is indicated in bold
red and is equal to 2297.

131  673  234  103  18
201  96   342  965  150
630  803  746  422  111
537  699  497  121  956
805  732  524  37   331

Find the minimal path sum, in matrix.txt (right click and "Save
Link/Target As..."), a 31K text file containing a 80 by 80 matrix,
from the top left to the bottom right by moving left, right, up, and down.

Antwoord: 425,185
"""

def _siftup(heap, pos):
    endpos = len(heap)
    startpos = pos
    newitem = heap[pos]
    childpos = 2*pos + 1    # leftmost child position
    while childpos < endpos:
        rightpos = childpos + 1
        if rightpos < endpos and not heap[childpos] < heap[rightpos]:
            childpos = rightpos
        heap[pos] = heap[childpos]
        pos = childpos
        childpos = 2*pos + 1
    heap[pos] = newitem
    _siftdown(heap, startpos, pos)

def _siftdown(heap, startpos, pos):
    newitem = heap[pos]
    while pos > startpos:
        parentpos = (pos - 1) >> 1
        parent = heap[parentpos]
        if newitem < parent:
            heap[pos] = parent
            pos = parentpos
            continue
        break
    heap[pos] = newitem

def heappush(heap, item):
    heap.append(item)
    _siftdown(heap, 0, len(heap)-1)

def heappop(heap):
    lastelt = heap.pop()    # raises appropriate IndexError if heap is empty
    if heap:
        returnitem = heap[0]
        heap[0] = lastelt
        _siftup(heap, 0)
    else:
        returnitem = lastelt
    return returnitem

class Graph(object):
    node_dict_factory = dict
    adjlist_outer_dict_factory = dict
    adjlist_inner_dict_factory = dict
    edge_attr_dict_factory = dict
    def __getstate__(self):
        attr = self.__dict__.copy()
        if 'nodes' in attr:
            del attr['nodes']
        if 'edges' in attr:
            del attr['edges']
        if 'degree' in attr:
            del attr['degree']
        return attr
    def __init__(self, incoming_graph_data=None, **attr):
        self.node_dict_factory = ndf = self.node_dict_factory
        self.adjlist_outer_dict_factory = self.adjlist_outer_dict_factory
        self.adjlist_inner_dict_factory = self.adjlist_inner_dict_factory
        self.edge_attr_dict_factory = self.edge_attr_dict_factory
        self.root_graph = self
        self.graph = {}   # dictionary for graph attributes
        self._node = ndf()  # empty node attribute dict
        self._adj = self.adjlist_outer_dict_factory()  # empty adjacency dict
        if incoming_graph_data is not None:
            to_networkx_graph(incoming_graph_data, create_using=self)
        self.graph.update(attr)
    @property
    def name(self):
        return self.graph.get('name', '')
    @name.setter
    def name(self, s):
        self.graph['name'] = s
    def __str__(self):
        return self.name
    def __iter__(self):
        return iter(self._node)
    def __contains__(self, n):
        try:
            return n in self._node
        except TypeError:
            return False
    def __len__(self):
        return len(self._node)
    def __getitem__(self, n):
        return self.adj[n]
    def add_node(self, node_for_adding, **attr):
        if node_for_adding not in self._node:
            self._adj[node_for_adding] = self.adjlist_inner_dict_factory()
            self._node[node_for_adding] = attr
        else:  # update attr even if node already exists
            self._node[node_for_adding].update(attr)
    def add_nodes_from(self, nodes_for_adding, **attr):
        for n in nodes_for_adding:
            try:
                if n not in self._node:
                    self._adj[n] = self.adjlist_inner_dict_factory()
                    self._node[n] = attr.copy()
                else:
                    self._node[n].update(attr)
            except TypeError:
                nn, ndict = n
                if nn not in self._node:
                    self._adj[nn] = self.adjlist_inner_dict_factory()
                    newdict = attr.copy()
                    newdict.update(ndict)
                    self._node[nn] = newdict
                else:
                    olddict = self._node[nn]
                    olddict.update(attr)
                    olddict.update(ndict)
    def remove_node(self, n):
        adj = self._adj
        try:
            nbrs = list(adj[n])  # list handles self-loops (allows mutation)
            del self._node[n]
        except KeyError:
            raise Exception("The node %s is not in the graph." % (n,))
        for u in nbrs:
            del adj[u][n]   # remove all edges n-u in graph
        del adj[n]          # now remove node
    def remove_nodes_from(self, nodes):
        adj = self._adj
        for n in nodes:
            try:
                del self._node[n]
                for u in list(adj[n]):   # list handles self-loops
                    del adj[u][n]  # (allows mutation of dict in loop)
                del adj[n]
            except KeyError:
                pass
    @property
    def nodes(self):
        nodes = NodeView(self)
        self.__dict__['nodes'] = nodes
        return nodes
    node = nodes
    def add_path(self, nodes, **attr):
        msg = "add_path is deprecated. Use nx.add_path instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.add_path(self, nodes, **attr)
    def add_cycle(self, nodes, **attr):
        msg = "add_cycle is deprecated. Use nx.add_cycle instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.add_cycle(self, nodes, **attr)
    def add_star(self, nodes, **attr):
        msg = "add_star is deprecated. Use nx.add_star instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.add_star(self, nodes, **attr)
    def nodes_with_selfloops(self):
        msg = "nodes_with_selfloops is deprecated." \
              "Use nx.nodes_with_selfloops instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.nodes_with_selfloops(self)
    def number_of_selfloops(self):
        msg = "number_of_selfloops is deprecated." \
              "Use nx.number_of_selfloops instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.number_of_selfloops(self)
    def selfloop_edges(self, data=False, keys=False, default=None):
        msg = "selfloop_edges is deprecated. Use nx.selfloop_edges instead."
        warnings.warn(msg, DeprecationWarning)
        return nx.selfloop_edges(self, data=False, keys=False, default=None)
    def number_of_nodes(self):
        return len(self._node)
    def order(self):
        return len(self._node)
    def has_node(self, n):
        try:
            return n in self._node
        except TypeError:
            return False
    def add_edge(self, u_of_edge, v_of_edge, **attr):
        u, v = u_of_edge, v_of_edge
        if u not in self._node:
            self._adj[u] = self.adjlist_inner_dict_factory()
            self._node[u] = {}
        if v not in self._node:
            self._adj[v] = self.adjlist_inner_dict_factory()
            self._node[v] = {}
        datadict = self._adj[u].get(v, self.edge_attr_dict_factory())
        datadict.update(attr)
        self._adj[u][v] = datadict
        self._adj[v][u] = datadict
    def add_edges_from(self, ebunch_to_add, **attr):
        for e in ebunch_to_add:
            ne = len(e)
            if ne == 3:
                u, v, dd = e
            elif ne == 2:
                u, v = e
                dd = {}  # doesnt need edge_attr_dict_factory
            else:
                raise Exception("Edge tuple %s must be a 2-tuple or 3-tuple." % (e,))
            if u not in self._node:
                self._adj[u] = self.adjlist_inner_dict_factory()
                self._node[u] = {}
            if v not in self._node:
                self._adj[v] = self.adjlist_inner_dict_factory()
                self._node[v] = {}
            datadict = self._adj[u].get(v, self.edge_attr_dict_factory())
            datadict.update(attr)
            datadict.update(dd)
            self._adj[u][v] = datadict
            self._adj[v][u] = datadict
    def add_weighted_edges_from(self, ebunch_to_add, weight='weight', **attr):
        self.add_edges_from(((u, v, {weight: d}) for u, v, d in ebunch_to_add), **attr)
    def remove_edge(self, u, v):
        try:
            del self._adj[u][v]
            if u != v:  # self-loop needs only one entry removed
                del self._adj[v][u]
        except KeyError:
            raise Exception("The edge %s-%s is not in the graph" % (u, v))

    def remove_edges_from(self, ebunch):
        adj = self._adj
        for e in ebunch:
            u, v = e[:2]  # ignore edge data if present
            if u in adj and v in adj[u]:
                del adj[u][v]
                if u != v:  # self loop needs only one entry removed
                    del adj[v][u]
    def has_edge(self, u, v):
        try:
            return v in self._adj[u]
        except KeyError:
            return False
    def neighbors(self, n):
        try:
            return iter(self._adj[n])
        except KeyError:
            raise Exception("The node %s is not in the graph." % (n,))
    def get_edge_data(self, u, v, default=None):
        try:
            return self._adj[u][v]
        except KeyError:
            return default
    def adjacency(self):
        return iter(self._adj.items())
    def clear(self):
        self._adj.clear()
        self._node.clear()
        self.graph.clear()
    def is_multigraph(self):
        return False
    def is_directed(self):
        return False
    def fresh_copy(self):
        return Graph()
    def to_directed(self, as_view=False):
        if as_view is True:
            return nx.graphviews.DiGraphView(self)
        G = DiGraph()
        G.graph.update(deepcopy(self.graph))
        G.add_nodes_from((n, deepcopy(d)) for n, d in self._node.items())
        G.add_edges_from((u, v, deepcopy(data))
                         for u, nbrs in self._adj.items()
                         for v, data in nbrs.items())
        return G
    def to_undirected(self, as_view=False):
        if as_view is True:
            return nx.graphviews.GraphView(self)
        G = Graph()
        G.graph.update(deepcopy(self.graph))
        G.add_nodes_from((n, deepcopy(d)) for n, d in self._node.items())
        G.add_edges_from((u, v, deepcopy(d))
                         for u, nbrs in self._adj.items()
                         for v, d in nbrs.items())
        return G
    def subgraph(self, nodes):
        induced_nodes = nx.filters.show_nodes(self.nbunch_iter(nodes))
        SubGraph = nx.graphviews.SubGraph
        if hasattr(self, '_NODE_OK'):
            return SubGraph(self._graph, induced_nodes, self._EDGE_OK)
        return SubGraph(self, induced_nodes)
    def edge_subgraph(self, edges):
        return nx.edge_subgraph(self, edges)
    def size(self, weight=None):
        s = sum(d for v, d in self.degree(weight=weight))
        return s // 2 if weight is None else s / 2
    def number_of_edges(self, u=None, v=None):
        if u is None:
            return int(self.size())
        if v in self._adj[u]:
            return 1
        return 0
    def nbunch_iter(self, nbunch=None):
        if nbunch is None:   # include all nodes via iterator
            bunch = iter(self._adj)
        elif nbunch in self:  # if nbunch is a single node
            bunch = iter([nbunch])
        else:                # if nbunch is a sequence of nodes
            def bunch_iter(nlist, adj):
                try:
                    for n in nlist:
                        if n in adj:
                            yield n
                except TypeError as e:
                    message = e.args[0]
                    # capture error for non-sequence/iterator nbunch.
                    if 'iter' in message:
                        msg = "nbunch is not a node or a sequence of nodes."
                        raise Exception(msg)
                    # capture error for unhashable node.
                    elif 'hashable' in message:
                        msg = "Node {} in sequence nbunch is not a valid node."
                        raise Exception(msg.format(n))
                    else:
                        raise
            bunch = bunch_iter(nbunch, self._adj)
        return bunch

class DiGraph(Graph):
    def __getstate__(self):
        attr = self.__dict__.copy()
        if 'nodes' in attr:
            del attr['nodes']
        if 'edges' in attr:
            del attr['edges']
        if 'out_edges' in attr:
            del attr['out_edges']
        if 'in_edges' in attr:
            del attr['in_edges']
        if 'degree' in attr:
            del attr['degree']
        if 'in_degree' in attr:
            del attr['in_degree']
        if 'out_degree' in attr:
            del attr['out_degree']
        return attr
    def __init__(self, incoming_graph_data=None, **attr):
        self.node_dict_factory = ndf = self.node_dict_factory
        self.adjlist_outer_dict_factory = self.adjlist_outer_dict_factory
        self.adjlist_inner_dict_factory = self.adjlist_inner_dict_factory
        self.edge_attr_dict_factory = self.edge_attr_dict_factory
        self.root_graph = self
        self.graph = {}  # dictionary for graph attributes
        self._node = ndf()  # dictionary for node attributes
        self._adj = ndf()  # empty adjacency dictionary
        self._pred = ndf()  # predecessor
        self._succ = self._adj  # successor
        if incoming_graph_data is not None:
            to_networkx_graph(incoming_graph_data, create_using=self)
        self.graph.update(attr)
    def add_node(self, node_for_adding, **attr):
        if node_for_adding not in self._succ:
            self._succ[node_for_adding] = self.adjlist_inner_dict_factory()
            self._pred[node_for_adding] = self.adjlist_inner_dict_factory()
            self._node[node_for_adding] = attr
        else:  # update attr even if node already exists
            self._node[node_for_adding].update(attr)
    def add_edge(self, u_of_edge, v_of_edge, **attr):
        u, v = u_of_edge, v_of_edge
        # add nodes
        if u not in self._succ:
            self._succ[u] = self.adjlist_inner_dict_factory()
            self._pred[u] = self.adjlist_inner_dict_factory()
            self._node[u] = {}
        if v not in self._succ:
            self._succ[v] = self.adjlist_inner_dict_factory()
            self._pred[v] = self.adjlist_inner_dict_factory()
            self._node[v] = {}
        # add the edge
        datadict = self._adj[u].get(v, self.edge_attr_dict_factory())
        datadict.update(attr)
        self._succ[u][v] = datadict
        self._pred[v][u] = datadict
    def has_successor(self, u, v):
        return (u in self._succ and v in self._succ[u])
    def has_predecessor(self, u, v):
        return (u in self._pred and v in self._pred[u])
    def successors(self, n):
        try:
            return iter(self._succ[n])
        except KeyError:
            raise Exception("The node %s is not in the digraph." % (n,))
    neighbors = successors
    def predecessors(self, n):
        try:
            return iter(self._pred[n])
        except KeyError:
            raise Exception("The node %s is not in the digraph." % (n,))
    def clear(self):
        self._succ.clear()
        self._pred.clear()
        self._node.clear()
        self.graph.clear()
    def is_multigraph(self):
        return False
    def is_directed(self):
        return True
    def fresh_copy(self):
        return DiGraph()
    def subgraph(self, nodes):
        induced_nodes = nx.filters.show_nodes(self.nbunch_iter(nodes))
        SubGraph = nx.graphviews.SubDiGraph
        if hasattr(self, '_NODE_OK'):
            return SubGraph(self._graph, induced_nodes, self._EDGE_OK)
        return SubGraph(self, induced_nodes)

def _dijkstra_multisource(G, sources, weight, pred=None, paths=None, cutoff=None, target=None):
    G_succ = G._succ if G.is_directed() else G._adj
    push = heappush
    pop = heappop
    dist = {}  # dictionary of final distances
    seen = {}
    c = count2()
    fringe = []
    for source in sources:
        seen[source] = 0
        push(fringe, (0, next(c), source))
    while fringe:
        (d, _, v) = pop(fringe)
        if v in dist:
            continue  # already searched this node.
        dist[v] = d
        if v == target:
            break
        for u, e in G_succ[v].items():
            cost = weight(v, u, e)
            if cost is None:
                continue
            vu_dist = dist[v] + cost
            if cutoff is not None:
                if vu_dist > cutoff:
                    continue
            if u in dist:
                if vu_dist < dist[u]:
                    raise ValueError('Contradictory paths found:', 'negative weights?')
            elif u not in seen or vu_dist < seen[u]:
                seen[u] = vu_dist
                push(fringe, (vu_dist, next(c), u))
                if paths is not None:
                    paths[u] = paths[v] + [u]
                if pred is not None:
                    pred[u] = [v]
            elif vu_dist == seen[u]:
                if pred is not None:
                    pred[u].append(v)
    return dist

def _dijkstra(G, source, weight, pred=None, paths=None, cutoff=None,
              target=None):
    return _dijkstra_multisource(G, [source], weight, pred=pred, paths=paths,
                                 cutoff=cutoff, target=target)

def _weight_function(G, weight):
    if callable(weight):
        return weight
    if G.is_multigraph():
        return lambda u, v, d: min(attr.get(weight, 1) for attr in d.values())
    return lambda u, v, data: data.get(weight, 1)

def dijkstra_path_length(G, source, target, weight='weight'):
    if source == target:
        return 0
    weight = _weight_function(G, weight)
    length = _dijkstra(G, source, weight, target=target)
    try:
        return length[target]
    except KeyError:
        raise nx.NetworkXNoPath(
            "Node %s not reachable from %s" % (target, source))

def problem83(fn = "euler83.txt"):
    matrix2 = [int(row) for row in open(fn).read().split()]
    matrix = [[0 for i in range(80)] for j in range(80)]
    for i, n in enumerate(matrix2): matrix[i // 80][i % 80] = n
    n, m = 80, 80
    G = DiGraph()
    for i in range(n):
        for j in range(m):
            neighbors = [(i+x, j+y) for x, y in ((-1,0), (0,-1), (1,0), (0,1))
                if 0 <= i+x < n and 0 <= j+y < m]
            for ix, jy in neighbors:
                G.add_edge((i, j), (ix, jy), weight = matrix[ix][jy])
    path_length = dijkstra_path_length(G, source=(0,0), target=(n-1,m-1))
    return path_length + matrix[0][0]

"""
#84: Monopoly odds

In the game, Monopoly, the standard board is set up in the following way:

A player starts on the GO square and adds the scores on two 6-sided dice
to determine the number of squares they advance in a clockwise direction.
Without any further rules we would expect to visit each square with equal
probability: 2.5%. However, landing on G2J (Go To Jail), CC (community
chest), and CH (chance) changes this distribution.

In addition to G2J, and one card from each of CC and CH, that orders the
player to go directly to jail, if a player rolls three consecutive
doubles, they do not advance the result of their 3rd roll. Instead they
proceed directly to jail.

At the beginning of the game, the CC and CH cards are shuffled. When a
player lands on CC or CH they take a card from the top of the respective
pile and, after following the instructions, it is returned to the bottom
of the pile. There are sixteen cards in each pile, but for the purpose of
this problem we are only concerned with cards that order a movement; any
instruction not concerned with movement will be ignored and the player
will remain on the CC/CH square.

    Community Chest (2/16 cards):
        Advance to GO
        Go to JAIL
    Chance (10/16 cards):
        Advance to GO
        Go to JAIL
        Go to C1
        Go to E3
        Go to H2
        Go to R1
        Go to next R (railway company)
        Go to next R
        Go to next U (utility company)
        Go back 3 squares.

The heart of this problem concerns the likelihood of visiting a particular
square. That is, the probability of finishing at that square after a roll.
For this reason it should be clear that, with the exception of G2J for
which the probability of finishing on it is zero, the CH squares will have
the lowest probabilities, as 5/8 request a movement to another square, and
it is the final square that the player finishes at on each roll that we are
interested in. We shall make no distinction between "Just Visiting" and
being sent to JAIL, and we shall also ignore the rule about requiring a
double to "get out of jail", assuming that they pay to get out on their
next turn.

By starting at GO and numbering the squares sequentially from 00 to 39 we
can concatenate these two-digit numbers to produce strings that correspond
with sets of squares.

Statistically it can be shown that the three most popular squares, in
order, are JAIL (6.24%) = Square 10, E3 (3.18%) = Square 24, and
GO (3.09%) = Square 00. So these three most popular squares can be listed
with the six-digit modal string: 102400.

If, instead of using two 6-sided dice, two 4-sided
dice are used, find the six-digit modal string.

Antwoord: 101,524
"""

class mersenne_rng(object):
    def __init__(self, seed = 5489):
        self.state = [0]*624
        self.f = 1812433253
        self.m = 397
        self.u = 11
        self.s = 7
        self.b = 0x9D2C5680
        self.t = 15
        self.c = 0xEFC60000
        self.l = 18
        self.index = 624
        self.lower_mask = (1<<31)-1
        self.upper_mask = 1<<31
        self.state[0] = seed
        for i in range(1,624):
            self.state[i] = self.int_32(self.f*(self.state[i-1]^(self.state[i-1]>>30)) + i)
    def twist(self):
        for i in range(624):
            temp = self.int_32((self.state[i]&self.upper_mask)+(self.state[(i+1)%624]&self.lower_mask))
            temp_shift = temp>>1
            if temp%2 != 0:
                temp_shift = temp_shift^0x9908b0df
            self.state[i] = self.state[(i+self.m)%624]^temp_shift
        self.index = 0
    def get_random_number(self):
        if self.index >= 624:
            self.twist()
        y = self.state[self.index]
        y = y^(y>>self.u)
        y = y^((y<<self.s)&self.b)
        y = y^((y<<self.t)&self.c)
        y = y^(y>>self.l)
        self.index+=1
        return self.int_32(y)
    def int_32(self, number):
        return int(0xFFFFFFFF & number)

board = ("GO", "A1", "CC1", "A2", "T1", "R1", "B1", "CH1", "B2", "B3", "JAIL",
    "C1", "U1", "C2", "C3", "R2", "D1", "CC2", "D2", "D3", "FP", "E1", "CH2",
    "E2", "E3", "R3", "F1", "F2", "U2", "F3", "G2J", "G1", "G2", "CC3", "G3",
    "R4", "CH3", "H1", "T2", "H2")

ccCards = ("GO", "JAIL", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10", "C11", "C12",
    "C13", "C14", "C15", "C16")

chCards = ("GO", "JAIL", "C1", "E3", "H2", "R1", "NR1", "NR2", "U", "BACK3",
    "C11", "C12", "C13", "C14", "C15", "C16")

class Monopoly:
    _pos = 0
    _doubles = 0
    hits = [0] * 40
    _rng = mersenne_rng(1131464071)
    def cc():
        i = 0
        while True:
            yield ccCards[i]
            i = (i + 1) % 16
    def ch():
        i = 0
        while True:
            yield chCards[i]
            i = (i + 1) % 16
    ccgen = cc()
    chgen = ch()
    def roll(self):
        dice1 = self._rng.get_random_number() % 4 + 1
        dice2 = self._rng.get_random_number() % 4 + 1
        dice = dice1 + dice2
        self._pos = (self._pos + dice) % 40
        self._doubles = self._doubles + 1 if dice1 == dice2 else 0
        if self._doubles == 3:
            self._doubles = 0
            self._pos = board.index("JAIL")
        if board[self._pos] in ["CH1", "CH2", "CH3"]:
            card = next(self.chgen)
            if card in ["GO", "JAIL", "C1", "E3", "H2", "R1"]:
                self._pos = board.index(card)
            if card in ["NR1", "NR2"]:  # next railroad
                if self._pos == board.index("CH1"): self._pos = board.index("R2")
                if self._pos == board.index("CH2"): self._pos = board.index("R3")
                if self._pos == board.index("CH3"): self._pos = board.index("R1")
            if card == "U":     # go to Utility
                if self._pos < board.index("U1") or self._pos > board.index("U2"):
                    self._pos = board.index("U1")
                if self._pos < board.index("U2") or self._pos > board.index("U1"):
                    self._pos = board.index("U2")
            if card == "BACK3": self._pos -= 3
        if board[self._pos] in ["CC1", "CC2", "CC3"]:
            card = next(self.ccgen)
            if card in ["GO", "JAIL"]:
                self._pos = board.index(card)
        if board[self._pos] == "G2J": self._pos = board.index("JAIL")
        self.hits[self._pos] += 1

def problem84():
    game = Monopoly()
    for i in range(10**6):
        game.roll()
    hits = game.hits
    srted = sorted(hits, reverse=True)
    ret = list()
    ret.append(hits.index(srted[0]))
    ret.append(hits.index(srted[1]))
    ret.append(hits.index(srted[2]))
    return concat2(ret)

"""
#85: Counting rectangles

Antwoord: 2,772
"""

def problem85():
    def rects(x, y):
        return ((x*x+x)*(y*y+y))//4
    best_x = best_y = best_rects = 9**9
    for x in range(1, 2001):
        for y in range(1, 2001):
            diff = abs(rects(x, y) - 2*10**6)
            if diff < best_rects:
                best_x, best_y, best_rects = x, y, diff
    return best_x * best_y

"""
#86: Cuboid route

A spider, S, sits in one corner of a cuboid room, measuring 6 by 5 by 3,
and a fly, F, sits in the opposite corner. By travelling on the surfaces
of the room the shortest "straight line" distance from S to F is 10 and
the path is shown on the diagram.

However, there are up to three "shortest" path candidates for any given
cuboid and the shortest route doesn't always have integer length.

It can be shown that there are exactly 2060 distinct cuboids, ignoring
rotations, with integer dimensions, up to a maximum size of M by M by M,
for which the shortest route has integer length when M = 100. This is the
least value of M for which the number of solutions first exceeds two
thousand; the number of solutions when M = 99 is 1975.

Find the least value of M such that the number
of solutions first exceeds one million.

Antwoord: 1,818
"""

"""
https://blog.dreamshire.com/project-euler-86-solution/
"""

def problem86():
    L, c, a = 10**6, 0, 2
    lprimes = list(sieve(16511080))
    while c < L:
        a += 1
        for bc in range(3, 2*a):
            if bc * a % 12 == 0:
                if issquare3(bc*bc + a*a):
                    c += min(bc, a + 1) - (bc + 1) // 2
    return a

"""
#87: Prime power triples

The smallest number expressible as the sum of a prime square, prime cube,
and prime fourth power is 28. In fact, there are exactly four numbers
below fifty that can be expressed in such a way:

28 = 22 + 23 + 24
33 = 32 + 23 + 24
49 = 52 + 23 + 24
47 = 22 + 33 + 24

How many numbers below fifty million can be expressed as the
sum of a prime square, prime cube, and prime fourth power?

Antwoord: 1,097,343
"""

def problem87():
    P = set()
    for a in sieve(7072):
        for b in sieve(369):
            for c in sieve(85):
                q  = a*a + b**3 + c**4
                if q >= 50000000: break
                P.add(q)
    return len(P)

"""
#88: Product-sum numbers

A natural number, N, that can be written as the sum and product of a
given set of at least two natural numbers, {a1, a2, ... , ak} is called
a product-sum number: N = a1 + a2 + ... + ak = a1 x a2 x ... x ak.

For example, 6 = 1 + 2 + 3 = 1 x 2 x 3.

For a given set of size, k, we shall call the smallest N with this
property a minimal product-sum number. The minimal product-sum numbers
for sets of size, k = 2, 3, 4, 5, and 6 are as follows.

k=2: 4 = 2 x 2 = 2 + 2
k=3: 6 = 1 x 2 x 3 = 1 + 2 + 3
k=4: 8 = 1 x 1 x 2 x 4 = 1 + 1 + 2 + 4
k=5: 8 = 1 x 1 x 2 x 2 x 2 = 1 + 1 + 2 + 2 + 2
k=6: 12 = 1 x 1 x 1 x 1 x 2 x 6 = 1 + 1 + 1 + 1 + 2 + 6

Hence for 2<=k<=6, the sum of all the minimal product-sum numbers is
4+6+8+12 = 30; note that 8 is only counted once in the sum.

In fact, as the complete set of minimal product-sum numbers
for 2<=k<=12 is {4, 6, 8, 12, 15, 16}, the sum is 61.

What is the sum of all the minimal product-sum numbers for 2<=k<=12000?

Antwoord: 7,587,457
"""

"""
https://blog.dreamshire.com/project-euler-88-solution/
"""

def problem88(kmax = 12000):
    def prodsum(p, s, c, start):
        k = p - s + c     # product - sum + number of factors
        if k < kmax:
            if p < n[k]: n[k] = p
            for i in range(start, kmax//p*2 + 1):
                prodsum(p*i, s+i, c+1, i)
    if kmax > 12: kmax+= 1
    n = [2*kmax] * kmax
    prodsum(1, 1, 1, 2)
    return sum(set(n[2:]))

"""
#89: Roman numerals

For a number written in Roman numerals to be considered valid there are
basic rules which must be followed. Even though the rules allow some
numbers to be expressed in more than one way there is always a "best"
way of writing a particular number.

For example, it would appear that there are
at least six ways of writing the number sixteen:

IIIIIIIIIIIIIIII
VIIIIIIIIIII
VVIIIIII
XIIIIII
VVVI
XVI

However, according to the rules only XIIIIII and XVI are valid, and the
last example is considered to be the most efficient, as it uses the least
number of numerals.

The 11K text file, roman.txt (right click and 'Save Link/Target As...'),
contains one thousand numbers written in valid, but not necessarily
minimal, Roman numerals; see About... Roman Numerals for the definitive
rules for this problem.

Find the number of characters saved by
writing each of these in their minimal form.

Note: You can assume that all the Roman numerals in the
file contain no more than four consecutive identical units.

Antwoord: 743
"""

"""
https://blog.dreamshire.com/project-euler-89-solution/
"""

"""
def problem89a(fn = "euler89.txt"):
    rows = open(fn).read()
    return len(rows) - len(re.sub("DCCCC|LXXXX|VIIII|CCCC|XXXX|IIII", '  ', rows))
"""

def problem89(fn = "euler89.txt"):
    txt1 = open(fn).read()
    txt2 = txt1.replace("DCCCC", "  ")
    txt3 = txt2.replace("LXXXX", "  ")
    txt4 = txt3.replace("VIIII", "  ")
    txt5 = txt4.replace("CCCC", "  ")
    txt6 = txt5.replace("XXXX", "  ")
    txt7 = txt6.replace("IIII", "  ")
    return len(txt1) - len(txt7)

"""
#90: Cube digit pairs

Antwoord: 1,217
"""

"""
https://blog.dreamshire.com/project-euler-90-solution/
"""

def problem90():
    squares = [(0,1), (0,4), (0,6), (1,6), (2,5), (3,6), (4,6), (8,1)]
    cube = list(combinations2([0,1,2,3,4,5,6,7,8,6], 6))
    valid = lambda c1, c2: all(x in c1 and y in c2 or x in c2 and y in c1 for x, y in squares)
    return sum(1 for i,c1 in enumerate(cube) for c2 in cube[:i] if valid(c1, c2))

"""
#91: Right triangles with integer coordinates

Antwoord: 14,234
"""

def problem91():
    n, t = 50, 0
    for x in range(1, n + 1):
        for y in range(1, n):
            m = gcd(x, y)
            t += min(x * m // y, m * (n - y) // x)
    return t * 2 + n * n * 3

"""
#92: Square digit chains

A number chain is created by continuously adding the square of the
digits in a number to form a new number until it has been seen before.

For example,

44 -> 32 -> 13 -> 10 -> 1 -> 1
85 -> 89 -> 145 -> 42 -> 20 -> 4 -> 16 -> 37 -> 58 -> 89

Therefore any chain that arrives at 1 or 89 will become stuck in an endless
loop. What is most amazing is that EVERY starting number will eventually
arrive at 1 or 89.

How many starting numbers below ten million will arrive at 89?

Antwoord: 8,581,146
"""

"""
https://blog.dreamshire.com/project-euler-92-solution/
"""

def problem92():
    def sos_digits(n):  # sum of squares of digits
        s = 0
        while n > 0:
            s, n = s + (n % 10)**2, n // 10
        return s
    def unhappy(n):
        while n > 1 and n != 89 and n != 4:
            n = sos_digits(n)
        return n>1
    L = 7    #  Limit is expressed as 10^L
    Lc = 9**2 * L + 1    #  maximum sum of digits square for L digits (plus 1)
    t = [0] * Lc
    solutions = [0] * Lc
    for i in range(10):
        solutions[i*i] = 1
    for i in range(2, L+1):
        for j in range(Lc):
            t[j] = sum(solutions[j - k*k] for k in range(10) if k*k <= j)
        solutions = list(t)
    return sum(solutions[i] for i in range(1, Lc) if unhappy(i))

def endchain(n):
    while n != 89 and n != 1:
        n = sum(d**2 for d in digits(n))
    return n

def problem92b():
    return sum(endchain(n) == 89 for n in range(1, 10**7))

"""
#93: Arithmetic expressions

By using each of the digits from the set, {1, 2, 3, 4}, exactly once, and
making use of the four arithmetic operations (+, −, *, /) and
brackets/parentheses, it is possible to form different positive integer targets.

For example,

8 = (4 * (1 + 3)) / 2
14 = 4 * (3 + 1 / 2)
19 = 4 * (2 + 3) − 1
36 = 3 * 4 * (2 + 1)

Note that concatenations of the digits, like 12 + 34, are not allowed.

Using the set, {1, 2, 3, 4}, it is possible to obtain thirty-one different
target numbers of which 36 is the maximum, and each of the numbers 1 to 28
can be obtained before encountering the first non-expressible number.

Find the set of four distinct digits, a < b < c < d, for which the longest
set of consecutive positive integers, 1 to n, can be obtained, giving your
answer as a string: abcd.

Antwoord: 1,258
"""

"""
https://blog.dreamshire.com/project-euler-93-solution/
"""

def product2(*args, repeat=1):
    pools = [tuple(pool) for pool in args] * repeat
    result = [[]]
    for pool in pools:
        result = [x+[y] for x in result for y in pool]
    for prod in result:
        yield tuple(prod)

add = lambda a, b: a + b
sub = lambda a, b: a - b
mul = lambda a, b: a * b
truediv = lambda a, b: a / b

def problem93():
    def seq_length(s, c=1):
        while c in s: c+= 1
        return c-1
    maxt, maxs = 0, 0
    for terms in combinations2(range(1, 10), 4):
        s = set()
        for n in permutations2(terms):
            for op in product2([add, mul, sub, truediv], repeat=3):
                x = op[0](op[1](n[0],n[1]),op[2](n[2],n[3]))    # (a.b).(c.d)
                if x%1 == 0 and x > 0: s.add(int(x))
                x = op[0](op[1](op[2](n[0],n[1]),n[2]),n[3])    # ((a.b).c).d
                if x%1 == 0 and x > 0: s.add(int(x))
                if seq_length(s) > maxs: maxs, maxt = seq_length(s), terms
    return concat2(maxt)

"""
#94: Almost equilateral triangles

It is easily proved that no equilateral triangle exists with integral
length sides and integral area. However, the almost equilateral triangle
5-5-6 has an area of 12 square units.

We shall define an almost equilateral triangle to be a triangle for which
two sides are equal and the third differs by no more than one unit.

Find the sum of the perimeters of all almost equilateral triangles with
integral side lengths and area and whose perimeters do not exceed one
billion (1,000,000,000).

Antwoord: 518,408,346
"""

"""
https://blog.dreamshire.com/project-euler-94-solution/
"""

def problem94():
    side0, side, s, p, m = 1, 1, 0, 0, 1
    L = 10**9
    while p <= L:
    	side0, side, m = side, 4*side - side0 + 2*m, -m
    	s += p
    	p = 3*side - m
    return s

"""
#95: Amicable chains

The proper divisors of a number are all the divisors excluding the number
itself. For example, the proper divisors of 28 are 1, 2, 4, 7, and 14. As
the sum of these divisors is equal to 28, we call it a perfect number.

Interestingly the sum of the proper divisors of 220 is 284 and the sum of
the proper divisors of 284 is 220, forming a chain of two numbers. For
this reason, 220 and 284 are called an amicable pair.

Perhaps less well known are longer chains. For example,
starting with 12496, we form a chain of five numbers:

12496 → 14288 → 15472 → 14536 → 14264 (→ 12496 → ...)

Since this chain returns to its starting
point, it is called an amicable chain.

Find the smallest member of the longest amicable
chain with no element exceeding one million.

Antwoord: 14,316
"""

"""
https://projecteuler.net/problem=95
"""

def problem95(L = 1000000):
    d = [1] * L
    for i in range(2, L//2):
        for j in range(2*i, L, i):
            d[j] += i
    max_cl = 0
    for i in range(2, L):
        n, chain = i, []
        while d[n] < L:
            d[n], n = L+1, d[n]
            try: k = chain.index(n)
            except ValueError: chain.append(n)
            else: 
                if len(chain[k:]) > max_cl:
                    max_cl, min_link = len(chain[k:]), min(chain[k:])
    return min_link

"""
#96: Su Doku

Su Doku (Japanese meaning number place) is the name given to a popular
puzzle concept. Its origin is unclear, but credit must be attributed to
Leonhard Euler who invented a similar, and much more difficult, puzzle
idea called Latin Squares. The objective of Su Doku puzzles, however, is
to replace the blanks (or zeros) in a 9 by 9 grid in such that each row,
column, and 3 by 3 box contains each of the digits 1 to 9. Below is an
example of a typical starting puzzle grid and its solution grid.

003  020  600
900  305  001
001  806  400

008  102  900
700  000  008
006  708  200

002  609  500
800  203  009
005  010  300

A well constructed Su Doku puzzle has a unique solution and can be solved
by logic, although it may be necessary to employ "guess and test" methods
in order to eliminate options (there is much contested opinion over this).
The complexity of the search determines the difficulty of the puzzle; the
example above is considered easy because it can be solved by straight
forward direct deduction.

The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'),
contains fifty different Su Doku puzzles ranging in difficulty, but all
with unique solutions (the first puzzle in the file is the example above).

By solving all fifty puzzles find the sum of the 3-digit numbers found in
the top left corner of each solution grid; for example, 483 is the 3-digit
number found in the top left corner of the solution grid above.

Antwoord: 24,702
"""

"""
http://norvig.com/sudoku.html
"""

def problem96(fn = "euler96.txt"):
    cross = lambda A, B: [a+b for a in A for b in B]
    digits2, rows = '123456789', 'ABCDEFGHI'
    cols = digits2
    squares = cross(rows, cols)
    unitlist = ([cross(rows, c) for c in cols] +
                [cross(r, cols) for r in rows] +
                [cross(rs, cs) for rs in ('ABC','DEF','GHI') for cs in ('123','456','789')])
    units = dict((s, [u for u in unitlist if s in u]) for s in squares)
    peers = dict((s, set(sum(units[s],[]))-set([s])) for s in squares)
    def eliminate(values, s, d):
        if d not in values[s]: return values
        values[s] = values[s].replace(d,'')
        if len(values[s]) == 0: return False
        elif len(values[s]) == 1:
            d2 = values[s]
            if not all(eliminate(values, s2, d2) for s2 in peers[s]):
                return False
        for u in units[s]:
            dplaces = [s for s in u if d in values[s]]
            if len(dplaces) == 0: return False
            elif len(dplaces) == 1:
                if not assign(values, dplaces[0], d): return False
        return values
    def assign(values, s, d):
        other_values = values[s].replace(d, '')
        return values if all(eliminate(values, s, d2) for d2 in other_values) else False
    def solve_all(grids):
        def search(values):
            def some(seq):
                for e in seq:
                    if e: return e
                return False
            if values is False: return False
            if all(len(values[s]) == 1 for s in squares): return values
            n,s = min((len(values[s]), s) for s in squares if len(values[s]) > 1)
            return some(search(assign(values.copy(), s, d)) for d in values[s])
        def parse_grid(grid):
            def grid_values(grid):
                chars = [c for c in grid if c in digits2 or c in '0.']
                assert len(chars) == 81
                return dict(zip(squares, chars))
            values = dict((s, digits2) for s in squares)
            for s,d in grid_values(grid).items():
                if d in digits2 and not assign(values, s, d):
                    return False
            return values
        ret = 0
        for grid in grids:
            values = search(parse_grid(grid))
            values2 = [int(n) for n in values.values()]
            ret += concat2(values2[:3])
        return ret
    puzzles = open(fn).read().strip().split()
    return solve_all(puzzles)

"""
#97: Large non-Mersenne prime

The first known prime found to exceed one million digits was discovered in
1999, and is a Mersenne prime of the form 26972593-1; it contains exactly
2,098,960 digits. Subsequently other Mersenne primes, of the form 2p-1,
have been found which contain more digits.

However, in 2004 there was found a massive non-Mersenne
prime which contains 2,357,207 digits: 28433x27830457+1.

Find the last ten digits of this prime number.

Antwoord: 8,739,992,577
"""

def problem97():
    return (28433*2**7830457+1)%10**10

"""
#98: Anagramic squares

By replacing each of the letters in the word CARE with 1, 2, 9, and 6
respectively, we form a square number: 1296 = 362. What is remarkable
is that, by using the same digital substitutions, the anagram, RACE,
also forms a square number: 9216 = 962. We shall call CARE (and RACE)
a square anagram word pair and specify further that leading zeroes are
not permitted, neither may a different letter have the same digital
value as another letter.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text
file containing nearly two-thousand common English words, find all the
square anagram word pairs (a palindromic word is NOT considered to be
an anagram of itself).

What is the largest square number formed by any member of such a pair?

NOTE: All anagrams formed must be contained in the given text file.

Antwoord: 18,769
"""

"""
https://blog.dreamshire.com/project-euler-98-solution/
"""

def problem98(fn = "euler98.txt"):
    def sq(n):
        x = int(''.join(y[letter_set[i]] for i in n))
        return x if int(x**0.5)**2 == x else False
    words = [(w[1:-1], sorted(w[1:-1]))  for w in open(fn).read().split(',') if len(w)>6]
    word_pairs = []
    while words:
        w = words.pop()
        word_pairs+= ((w[0], a[0]) for a in words if w[1] == a[1])
    max_sq = 0
    for w, a in word_pairs:
        letter_set = {x:y for y, x in enumerate(set(w))}
        for y in permutations2('123456789', len(letter_set)):
            if sq(w) and sq(a): max_sq = max(sq(w), sq(a), max_sq)
    return max_sq

"""
#99: Largest exponential

Comparing two numbers written in index form like 211 and 37 is not
difficult, as any calculator would confirm that 211 = 2048 < 37 = 2187.

However, confirming that 632382518061 > 519432525806 would be much
more difficult, as both numbers contain over three million digits.

Using base_exp.txt (right click and 'Save Link/Target As...'), a 22K text
file containing one thousand lines with a base/exponent pair on each line,
determine which line number has the greatest numerical value.

NOTE: The first two lines in the file represent the numbers in the example given above.

Antwoord: 709
"""

"""
https://blog.dreamshire.com/project-euler-99-solution/
"""

def ln2(x):
    n = 99000.0
    return n * ((x ** (1/n)) - 1)

def problem99():
    pairs = open("euler99.txt").read().split('\n')
    mv, ml = 0, 0 
    for ln, line in enumerate(pairs, start=1):
        b, e = line.split(',') 
        v = int(e) * ln2(int(b)) 
        if v > mv: mv, ml = v, ln 
    return ml

"""
#100: Arranged probability

If a box contains twenty-one coloured discs, composed of fifteen blue discs
and six red discs, and two discs were taken at random, it can be seen that
the probability of taking two blue discs, P(BB) = (15/21)×(14/20) = 1/2.

The next such arrangement, for which there is exactly 50% chance of taking
two blue discs at random, is a box containing eighty-five blue discs and
thirty-five red discs.

By finding the first arrangement to contain over 1012 = 1,000,000,000,000
discs in total, determine the number of blue discs that the box would contain.

Antwoord: 756,872,327,473
"""

"""
https://blog.dreamshire.com/project-euler-100-solution/
"""

def problem100():
    b, n, L = 3, 4, 10**12
    while n <= L: b, n = 3*b + 2*n - 2, 4*b + 3*n - 3
    return b

"""
#101: Optimum polynomial

If we are presented with the first k terms of a sequence it is impossible
to say with certainty the value of the next term, as there are infinitely
many polynomial functions that can model the sequence.

As an example, let us consider the sequence of cube numbers. This is
defined by the generating function,
un = n3: 1, 8, 27, 64, 125, 216, ...

Suppose we were only given the first two terms of this sequence. Working
on the principle that "simple is best" we should assume a linear
relationship and predict the next term to be 15 (common difference 7).
Even if we were presented with the first three terms, by the same
principle of simplicity, a quadratic relationship should be assumed.

We shall define OP(k, n) to be the nth term of the optimum polynomial
generating function for the first k terms of a sequence. It should be
clear that OP(k, n) will accurately generate the terms of the sequence
for n ≤ k, and potentially the first incorrect term (FIT) will be
OP(k, k+1); in which case we shall call it a bad OP (BOP).

As a basis, if we were only given the first term of sequence, it would be
most sensible to assume constancy; that is, for n ≥ 2, OP(1, n) = u1.

Hence we obtain the following OPs for the cubic sequence:
OP(1, n) = 1  1, 1, 1, 1, ...
OP(2, n) = 7n−6  1, 8, 15, ...
OP(3, n) = 6n2−11n+6       1, 8, 27, 58, ...
OP(4, n) = n3  1, 8, 27, 64, 125, ...

Clearly no BOPs exist for k ≥ 4.

By considering the sum of FITs generated by the BOPs
(indicated in red above), we obtain 1 + 15 + 58 = 74.

Consider the following tenth degree polynomial generating function:

un = 1 − n + n2 − n3 + n4 − n5 + n6 − n7 + n8 − n9 + n10

Find the sum of FITs for the BOPs.

Antwoord: 37,076,114,526
"""

"""
https://blog.dreamshire.com/project-euler-101-solution/
"""

def problem101():
    k = 10
    u = lambda n: 1 - n + n**2 - n**3 + n**4 - n**5 + n**6 - n**7 + n**8 - n**9 + n**10
    def tld(x):
        return [x[i] - x[i - 1] for i in range(1, len(x))]
    seq = [list(map(u, range(1, k + 1)))]
    for i in range(k):
        seq += [tld(seq[-1])]
    return sum(sum(seq, []))

"""
#102: Triangle containment

Three distinct points are plotted at random on a Cartesian plane,
for which -1000 ≤ x, y ≤ 1000, such that a triangle is formed.

Consider the following two triangles:

A(-340,495), B(-153,-910), C(835,-947)

X(-175,41), Y(-421,-714), Z(574,-645)

It can be verified that triangle ABC contains
the origin, whereas triangle XYZ does not.

Using triangles.txt (right click and 'Save Link/Target As...'), a 27K
text file containing the co-ordinates of one thousand "random" triangles,
find the number of triangles for which the interior contains the origin.

NOTE: The first two examples in the file represent
the triangles in the example given above.

Antwoord: 228
"""

"""
https://blog.dreamshire.com/project-euler-102-solution/
"""

def problem102():
    C = 0
    for line in open('euler102.txt'):
        ax, ay, bx, by, cx, cy = map(int, line.split(','))
        a = ax*by - ay*bx > 0
        b = bx*cy - by*cx > 0
        c = cx*ay - cy*ax > 0
        C+= a==b==c
    return C

"""
Einde opdrachten
"""

def runn2(n = 1):
    if n == 1: return problem1()
    if n == 2: return problem2()
    if n == 3: return problem3()
    if n == 4: return problem4()
    if n == 5: return problem5()
    if n == 6: return problem6()
    if n == 7: return problem7()
    if n == 8: return problem8()
    if n == 9: return problem9()
    if n == 10: return problem10()
    if n == 11: return problem11()
    if n == 12: return problem12()
    if n == 13: return problem13()
    if n == 14: return problem14()
    if n == 15: return problem15()
    if n == 16: return problem16()
    if n == 17: return problem17()
    if n == 18: return problem18()
    if n == 19: return problem19()
    if n == 20: return problem20()
    if n == 21: return problem21()
    if n == 22: return problem22()
    if n == 23: return problem23()
    if n == 24: return problem24()
    if n == 25: return problem25()
    if n == 26: return problem26()
    if n == 27: return problem27()
    if n == 28: return problem28()
    if n == 29: return problem29()
    if n == 30: return problem30()
    if n == 31: return problem31()
    if n == 32: return problem32()
    if n == 33: return problem33()
    if n == 34: return problem34()
    if n == 35: return problem35()
    if n == 36: return problem36()
    if n == 37: return problem37()
    if n == 38: return problem38()
    if n == 39: return problem39()
    if n == 40: return problem40()
    if n == 41: return problem41()
    if n == 42: return problem42()
    if n == 43: return problem43()
    if n == 44: return problem44()
    if n == 45: return problem45()
    if n == 46: return problem46()
    if n == 47: return problem47()
    if n == 48: return problem48()
    if n == 49: return problem49()
    if n == 50: return problem50()
    if n == 51: return problem51()
    if n == 52: return problem52()
    if n == 53: return problem53()
    if n == 54: return problem54()
    if n == 55: return problem55()
    if n == 56: return problem56()
    if n == 57: return problem57()
    if n == 58: return problem58()
    if n == 59: return problem59()
    if n == 60: return problem60()
    if n == 61: return problem61()
    if n == 62: return problem62()
    if n == 63: return problem63()
    if n == 64: return problem64()
    if n == 65: return problem65()
    if n == 66: return problem66()
    if n == 67: return problem67()
    if n == 68: return problem68()
    if n == 69: return problem69()
    if n == 70: return problem70()
    if n == 71: return problem71()
    if n == 72: return problem72()
    if n == 73: return problem73()
    if n == 74: return problem74()
    if n == 75: return problem75()
    if n == 76: return problem76()
    if n == 77: return problem77()
    if n == 78: return problem78()
    if n == 79: return problem79()
    if n == 80: return problem80()
    if n == 81: return problem81()
    if n == 82: return problem82()
    if n == 83: return problem83()
    if n == 84: return problem84()
    if n == 85: return problem85()
    if n == 86: return problem86()
    if n == 87: return problem87()
    if n == 88: return problem88()
    if n == 89: return problem89()
    if n == 90: return problem90()
    if n == 91: return problem91()
    if n == 92: return problem92()
    if n == 93: return problem93()
    if n == 94: return problem94()
    if n == 95: return problem95()
    if n == 96: return problem96()
    if n == 97: return problem97()
    if n == 98: return problem98()
    if n == 99: return problem99()
    if n == 100: return problem100()
    if n == 101: return problem101()
    if n == 102: return problem102()
    return 0

answers = [233168, 4613732, 6857, 906609, 232792560, 25164150, 104743, 23514624000,
    31875000, 142913828922, 70600674, 76576500, 5537376230, 837799, 137846528820,
    1366, 21124, 1074, 171, 648, 31626, 871198282, 4179871, 2783915460, 4782, 983,
    -59231, 669171001, 9183, 443839, 73682, 45228, 100, 40730, 55, 872187, 748317,
    932718654, 840, 210, 7652413, 162, 16695334890, 5482660, 1533776805, 5777,
    134043, 9110846700, 296962999629, 997651, 121313, 142857, 4075, 376, 249, 972,
    153, 26241, 107359, 26033, 28684, 127035954683, 49, 1322, 272, 661, 7273,
    6531031914842725, 510510, 8319823, 428570, 303963552391, 7295372, 402, 161667,
    381138582, 71, 55374, 73162890, 40886, 427337, 260324, 425185, 101524, 2772, 1818,
    1097343, 7587457, 743, 1217, 14234, 8581146, 1258, 518408346, 14316, 24702,
    8739992577, 18769, 709, 756872327473, 37076114526, 228]

#answers[61 - 1] = 0

import time
import math
import concurrent.futures

def runjob(n):
    ts = time.time()
    ret = runn2(n)
    assert ret == answers[n - 1]
    print("#{}: {} {}s".format(n, ret, math.floor(time.time() - ts)))

def runm(l = list(range(1, 102 + 1))):
    ts = time.time()
    with concurrent.futures.ProcessPoolExecutor() as executor:
        executor.map(runjob, l)
    print("Total: {}s".format(math.floor(time.time() - ts)))

def runs(l = range(1, 102 + 1)):
    ts = time.time()
    for job in l:
        runjob(job)
    print("Total: {}s".format(math.floor(time.time() - ts)))


