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
    for i in range(2, reducer2(n, presets)):
        if n % i == 0: return False
    return True

#def triangle(n): return n * (n + 1) >> 1
#def pentagon(n): return n * (3 * n - 1)//2
hexagon = lambda n: n * (2 * n - 1)

def polygonizer(limit, n = 1):
    step = 1
    ret = 0
    while ret < limit:
        yield ret
        ret += step
        step += n

def binSearch(d, n):
    if n > d[-1]: return False
    first, last = 0, len(d)
    middle = (first + last) // 2
    while first <= last:
        if d[middle] < n: first = middle + 1
        elif d[middle] == n: return True
        else: last = middle - 1
        middle = (first + last) // 2
    return False

def decimals(n):
    i = 0
    while n: n = n // 10; i += 1
    return i

def digits(n):
    while n: yield n % 10; n = n // 10;

digit = lambda n, i: n // 10**i % 10

def product(l):
    xsum = 1
    for n in l: xsum *= n
    return xsum

factorial = lambda n: product(range(2, n + 1))
combinations = lambda n, r: factorial(n) // (factorial(r) * factorial(n - r))

def reverse(n, base = 10):
    temp, rev = n, 0
    while temp != 0:
        rev = rev * base + temp % base
        temp = temp // base
    return rev

ispalindrome = lambda n, base = 10: n == reverse(n, base)

def hasOnce(n, nset):
    for i in n:
        if i in nset: nset.remove(i)
        else: return False
    return True

hasDigitsOnce = lambda n, nset: hasOnce(digits(n), nset)

def isPandigital(n):
    return hasDigitsOnce(n, list(range(1, decimals(n) + 1)))

def kounter(hand):
    dct = dict()
    for card in hand:
        if card in dct: dct[card] += 1
        else: dct[card] = 1
    return dct

frequencies = lambda hand: (x[1] for x in kounter(hand).items())

def primefactor(primes, n):
    for prime in primes:
        if n % prime == 0: return prime
    raise Exception("Ran out of primes!")

def primefactors(primes, n):
    while True:
        factor = primefactor(primes, n)
        yield factor
        if factor == n: break
        n = n // factor

# yields proper divisors
def properDivs(n):
    for i in range(1, n // 2 + 1):
        if n % i == 0: yield i

# yields all divisors (proper divisors + number itself)
def divisors(n):
    for div in properDivs(n): yield div
    yield n

# number of all divisors
def n_divs1(n):
    if n <= 2: return n
    return len(list(divisors(n)))

# number of all divisors using prime factorization
def n_divs2(lprimes, n):
    if n <= 2: return n
    pfactors = primefactors(lprimes, n)
    return product(x + 1 for x in frequencies(pfactors))

# sum of proper divisors
sumProperDivs1 = lambda n: sum(properDivs(n))

# sum of all divisors
def sum_divs1(n):
    return sumProperDivs1(n) + n

# sum of all divisors using prime factorization
def sum_divs2(lprimes, n):
    previous, ret, current = 0, 1, 1
    for p in primefactors(lprimes, n):
        if p != previous:
            ret *= sum_divs1(current)
            current = 1
        current *= p
        previous = p
    return ret * sum_divs1(current)

# sum of all divisors using prime factorization
def sum_divs3(lprimes, n):
    return product(sum_divs1(key ** value)
        for key, value in kounter(primefactors(lprimes, n)).items())

# sum of proper divisors using prime factorization
def sumProperDivs2(lprimes, n):
    return sum_divs2(lprimes, n) - n

def fibonacci(xmax, term1 = 1, term2 = 2):
    yield term1
    yield term2
    while term1 + term2 <= xmax:
        yield term1 + term2
        term1, term2 = term2, term1 + term2

import random

def miller_rabin(n, k = 3):
    if n < 6:  # assuming n >= 0 in all cases... shortcut small cases here
        return [False, False, True, True, False, True][n]
    if n % 2 == 0:  # should be faster than n % 2
        return False
    s, d = 0, n - 1
    while d % 2 == 0:
        s, d = s + 1, d >> 1
    for a in random.sample(range(2, n-2), k):
        x = pow(a, d, n)
        if x != 1 and x + 1 != n:
            for r in range(1, s):
                x = pow(x, 2, n)
                if x == 1:
                    return False  # composite for sure
                elif x == n - 1:
                    a = 0  # so we know loop didn't continue to end
                    break  # could be strong liar, try another a
            if a:
                return False  # composite if we reached end of this loop
    return True  # probably prime if reached end of outer loop


"""
#1 Multiples of 3 and 5

If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

Antwoord: 233,168
"""
opdracht1 = lambda limit = 1000: sum(set(range(3,limit,3)) | set(range(5,limit,5)))

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

opdracht2 = lambda xmax = 4*10**6: sum(x for x in fibonacci(xmax) if x % 2 == 0)

"""
#3 Largest prime factor

The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
"""

"""
71*839*1,471*6,857=600,851,475,143
"""

opdracht3 = lambda n = 600851475143: max(primefactors(list(sieve(7000)), n))

"""
#4 Largest palindrome product

A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
"""

def opdracht4(x = range(100, 1000), y = range(999, 99, -1)):
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

def opdracht5(l = (11,12,13,14,15,16,17,18,19,20)):
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
natural numbers and the square of the sum is 3025 − 385 = 2640.

Find the difference between the sum of the squares of the
first one hundred natural numbers and the square of the sum.

Antwoord: 25,164,150
"""

def opdracht6(r = range(1, 101)):
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

def opdracht7(n = 10001):
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
have the greatest product are 9 × 9 × 8 × 9 = 5832.

73167176531330624919225119674426574742355349194934
96983520312774506326239578318016984801869478851843
85861560789112949495459501737958331952853208805511
12540698747158523863050715693290963295227443043557
66896648950445244523161731856403098711121722383113
62229893423380308135336276614282806444486645238749
30358907296290491560440772390713810515859307960866
70172427121883998797908792274921901699720888093776
65727333001053367881220235421809751254540594752243
52584907711670556013604839586446706324415722155397
53697817977846174064955149290862569321978468622482
83972241375657056057490261407972968652414535100474
82166370484403199890008895243450658541227588666881
16427171479924442928230863465674813919123162824586
17866458359124566529476545682848912883142607690042
24219022671055626321111109370544217506941658960408
07198403850962455444362981230987879927244284909188
84580156166097919133875499200524063689912560717606
05886116467109405077541002256983155200055935729725
71636269561882670428252483600823257530420752963450

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

def opdracht8(series1 = series8):
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

def opdracht9(search = 1000):
    for a in range(1,search - 1):
        for b in range(1,search - a):
            c = search - a - b
            if a * a + b * b == c * c:
                return a * b * c
    return 0

"""
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
"""

opdracht10 = lambda limit = 2*10**6: sum(sieve(limit))

"""
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

def opdracht11(data = t11):
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

def opdracht12(ndivs = 500):
    lprimes = list(sieve(10**5))
    for n in polygonizer(10**8, 1):
        if n_divs2(lprimes, n) > ndivs: return n
    raise Exception("Answer not found")
    return 0

"""
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

37107287533902102798797998220837590246510135740250
46376937677490009712648124896970078050417018260538
74324986199524741059474233309513058123726617309629
91942213363574161572522430563301811072406154908250
23067588207539346171171980310421047513778063246676
89261670696623633820136378418383684178734361726757
28112879812849979408065481931592621691275889832738
44274228917432520321923589422876796487670272189318
47451445736001306439091167216856844588711603153276
70386486105843025439939619828917593665686757934951
62176457141856560629502157223196586755079324193331
64906352462741904929101432445813822663347944758178
92575867718337217661963751590579239728245598838407
58203565325359399008402633568948830189458628227828
80181199384826282014278194139940567587151170094390
35398664372827112653829987240784473053190104293586
86515506006295864861532075273371959191420517255829
71693888707715466499115593487603532921714970056938
54370070576826684624621495650076471787294438377604
53282654108756828443191190634694037855217779295145
36123272525000296071075082563815656710885258350721
45876576172410976447339110607218265236877223636045
17423706905851860660448207621209813287860733969412
81142660418086830619328460811191061556940512689692
51934325451728388641918047049293215058642563049483
62467221648435076201727918039944693004732956340691
15732444386908125794514089057706229429197107928209
55037687525678773091862540744969844508330393682126
18336384825330154686196124348767681297534375946515
80386287592878490201521685554828717201219257766954
78182833757993103614740356856449095527097864797581
16726320100436897842553539920931837441497806860984
48403098129077791799088218795327364475675590848030
87086987551392711854517078544161852424320693150332
59959406895756536782107074926966537676326235447210
69793950679652694742597709739166693763042633987085
41052684708299085211399427365734116182760315001271
65378607361501080857009149939512557028198746004375
35829035317434717326932123578154982629742552737307
94953759765105305946966067683156574377167401875275
88902802571733229619176668713819931811048770190271
25267680276078003013678680992525463401061632866526
36270218540497705585629946580636237993140746255962
24074486908231174977792365466257246923322810917141
91430288197103288597806669760892938638285025333403
34413065578016127815921815005561868836468420090470
23053081172816430487623791969842487255036638784583
11487696932154902810424020138335124462181441773470
63783299490636259666498587618221225225512486764533
67720186971698544312419572409913959008952310058822
95548255300263520781532296796249481641953868218774
76085327132285723110424803456124867697064507995236
37774242535411291684276865538926205024910326572967
23701913275725675285653248258265463092207058596522
29798860272258331913126375147341994889534765745501
18495701454879288984856827726077713721403798879715
38298203783031473527721580348144513491373226651381
34829543829199918180278916522431027392251122869539
40957953066405232632538044100059654939159879593635
29746152185502371307642255121183693803580388584903
41698116222072977186158236678424689157993532961922
62467957194401269043877107275048102390895523597457
23189706772547915061505504953922979530901129967519
86188088225875314529584099251203829009407770775672
11306739708304724483816533873502340845647058077308
82959174767140363198008187129011875491310547126581
97623331044818386269515456334926366572897563400500
42846280183517070527831839425882145521227251250327
55121603546981200581762165212827652751691296897789
32238195734329339946437501907836945765883352399886
75506164965184775180738168837861091527357929701337
62177842752192623401942399639168044983993173312731
32924185707147349566916674687634660915035914677504
99518671430235219628894890102423325116913619626622
73267460800591547471830798392868535206946944540724
76841822524674417161514036427982273348055556214818
97142617910342598647204516893989422179826088076852
87783646182799346313767754307809363333018982642090
10848802521674670883215120185883543223812876952786
71329612474782464538636993009049310363619763878039
62184073572399794223406235393808339651327408011116
66627891981488087797941876876144230030984490851411
60661826293682836764744779239180335110989069790714
85786944089552990653640447425576083659976645795096
66024396409905389607120198219976047599490197230297
64913982680032973156037120041377903785566085089252
16730939319872750275468906903707539413042652315011
94809377245048795150954100921645863754710598436791
78639167021187492431995700641917969777599028300699
15368713711936614952811305876380278410754449733078
40789923115535562561142322423255033685442488917353
44889911501440648020369068063960672322193204149535
41503128880339536053299340368006977710650566631954
81234880673210146739058568557934581403627822703280
82616570773948327592232845941706525094512325230608
22918802058777319719839450180888072429661980811197
77158542502016545090413245809786882778948721859617
72107838435069186155435662884062257473692284509516
20849603980134001723930671666823555245252804609722
53503534226472524250874054075591789781264330331690

Antwoord: 5,537,376,230
"""

def opdracht13(fn = "euler13.txt"):
    return sum(int(n) for n in open(fn).read().splitlines() if len(n) > 0) // 10**42
            
"""
#14 Longest Collatz sequence

The following iterative sequence is defined for the set of positive integers:

n → n/2 (n is even)
n → 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:
13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1

It can be seen that this sequence (starting at 13 and finishing at 1)
contains 10 terms. Although it has not been proved yet (Collatz Problem),
it is thought that all starting numbers finish at 1.

Which starting number, under one million, produces the longest chain?

NOTE: Once the chain starts the terms are allowed to go above one million.

Antwoord: 837,799
"""

def opdracht14(limit = 10**6):
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

How many such routes are there through a 20×20 grid?

Antwoord: 137,846,528,820
"""

opdracht15 = lambda size = 20: combinations(2*size, size)

"""
#16 Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Antwoord: 1366
"""

opdracht16 = lambda e = 1000: sum(digits(2**e))

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

def opdracht17():
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
"""

triangle18 = (
    (75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (95,64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (17,47,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (18,35,87,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (20, 4,82,47,65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (19, 1,23,75, 3,34, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (88, 2,77,73, 7,63,67, 0, 0, 0, 0, 0, 0, 0, 0),
    (99,65, 4,28, 6,16,70,92, 0, 0, 0, 0, 0, 0, 0),
    (41,41,26,56,83,40,80,70,33, 0, 0, 0, 0, 0, 0),
    (41,48,72,33,47,32,37,16,94,29, 0, 0, 0, 0, 0),
    (53,71,44,65,25,43,91,52,97,51,14, 0, 0, 0, 0),
    (70,11,33,28,77,73,17,78,39,68,17,57, 0, 0, 0),
    (91,71,52,38,17,14,91,43,58,50,27,29,48, 0, 0),
    (63,66, 4,68,89,53,67,30,73,16,69,87,40,31, 0),
    ( 4,62,98,27,23, 9,70,98,73,93,38,53,60, 4,23));

def opdracht18(triangle = triangle18):
    root = len(triangle[0])
    possibilities = 2**(root-1)
    best = 0
    for i in range(0, possibilities + 1):
        index, xsum = 0, triangle[0][0]
        for j in range(0, root - 1):
            index = index + (i >> j & 1)
            value = triangle[j + 1][index]
            xsum += value
        best = max(best, xsum)
    return best

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

def opdracht19():
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

n! means n × (n − 1) × ... × 3 × 2 × 1

For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!

Antwoord: 648
"""

opdracht20 = lambda f = 100: sum(digits(factorial(f)))

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

Antwoord: 31626
"""

"""
220 - 284
1184 - 1210
2620 - 2924
5020 - 5564
6232 - 6368
"""

def opdracht21(low = 1, high = 10**4):
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
COLIN would obtain a score of 938 × 53 = 49714.

What is the total of all the name scores in the file?

Antwoord: 871,198,282
"""

def opdracht22():
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

def opdracht23():
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

def opdracht24(pool = [0,1,2,3,4,5,6,7,8,9], perm = 1000000 - 1):
    lst = list()
    while len(pool) > 0:
        i, perm = divmod(perm, factorial(len(pool) - 1))
        lst.append(pool[i])
        pool.pop(i)
    return sum(n*10**i for i, n in enumerate(reversed(lst)))

"""
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
"""

def opdracht25(limit = 10**999):
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

def opdracht26b():
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

def opdracht26():
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
"""

def opdracht27():
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

def opdracht28(root = 1001):
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

Consider all integer combinations of a^b for 2 ≤ a ≤ 5 and 2 ≤ b ≤ 5:

2^2=4, 2^3=8, 2^4=16, 2^5=32
3^2=9, 3^3=27, 3^4=81, 3^5=243
4^2=16, 4^3=64, 4^4=256, 4^5=1024
5^2=25, 5^3=125, 5^4=625, 5^5=3125

If they are then placed in numerical order, with any repeats
removed, we get the following sequence of 15 distinct terms:

4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125

How many distinct terms are in the sequence
generated by ab for 2 ≤ a ≤ 100 and 2 ≤ b ≤ 100?

Antwoord: 9,183
"""

def opdracht29(ar = range(2, 100+1), br = range(2,100+1)):
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

def opdracht30(p = 5):
    def test(n, p): return n if sum(d**p for d in digits(n)) == n else 0
    return sum(test(i, p) for i in range(2, 10**6))

"""
#31: Coin sums

In England the currency is made up of pound, P, and pence,
p, and there are eight coins in general circulation:

    1p, 2p, 5p, 10p, 20p, 50p, P1 (100p) and P2 (200p).

It is possible to make £2 in the following way:

    1×P1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p

How many different ways can £2 be made using any number of coins?

Antwoord: 73,682
"""

def opdracht31(target = 200, coins = (1,2,5,10,20,50,100,200)):
    ways = [1] + [0]*target
    for coin in coins:
        for i in range(coin, target + 1):
            ways[i] += ways[i - coin]
    return ways[target]

"""
#32: Pandigital products

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once; for example, the 5-digit number, 15234,
is 1 through 5 pandigital.

The product 7254 is unusual, as the identity, 39 × 186 = 7254, containing
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
45,228
"""

def opdracht32():
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
def gcd(a, b):
    while b: a, b = b, a % b
    return a

def opdracht33():
    dp, np = 1,1
    for c in range(1,10):
        for d in range(1,c):
            for n in range(1,d):
                if (n * 10 + c) * d == (c * 10 + d) * n:
                    np *= n
                    dp *= d
    return dp // gcd(np, dp)

def opdracht33b():
    d = 1
    for i in range(1, 10):
        for j in range(1, i):
            q, r = divmod(9*j*i, 10*j-i)
            if not r and q <= 9:
                print("{}/{}".format(i, j))
                d*= i/j
    return int(d)

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

def opdracht34():
    factorials = (1,1,2,6,24,120,720,5040,40320,362880);
    def facsumdig(n): return sum(factorials[x] for x in digits(n));
    return sum(k for k in range(10, factorials[9] * 7) if facsumdig(k) == k)

"""
#35: Circular primes

The number, 197, is called a circular prime because all rotations
of the digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

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

def opdracht35(xmax = 10**6-1):
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

The decimal number, 585 = 1001001001_2 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base, may not include leading zeros.)

Antwoord: 872,187
"""

"""
1 + 3 + 5 + 7 + 9 + 33 + 99 + 313 + 585 + 717 + 7447 + 9009 +
15351 + 32223 + 39993 + 53235 + 53835 + 73737 + 585585 = 872187
"""

def opdracht36(r = range(1,10**6)):
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

def opdracht37():
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
"""

"""
9,327*10^5 + 2*9,327 = 9,327*10^5 + 18,654 = 932,718,654
"""

def opdracht38():
    def concat(a, b): return b + a * 10**decimals(b)
    for i in range(9387, 9234, -1):
        result = concat(i, 2 * i)
        if isPandigital(result):
            return result
    return 0

"""
#39: Integer right triangles

If p is the perimeter of a right angle triangle with integral length
sides, {a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

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

def opdracht39():
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

d1 × d10 × d100 × d1000 × d10000 × d100000 × d1000000

Antwoord: 210
"""

"""
1*1*5*3*7*2*1=210
"""

def opdracht40(indices = [0,9,99,999,9999,99999,999999]):
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

def opdracht41(limit = 7654321):
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

def opdracht42(fn = "euler42.txt"):
    words = [w for w in open(fn).read().splitlines() if len(w) > 0]
    triangles = set(polygonizer(999, 1))   # 999 is arbitrary
    def wordcount(word):
        return sum(ord(letter) - 64 for letter in word)
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

def opdracht43():
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
    def concat(lst):
        ret = 0
        for i, n in enumerate(reversed(lst)):
            ret += n *10**i
        return ret
    def perms(a):
        for p in permutations(a):
            p.insert(5,5)
            yield concat(p);
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
Pn=n(3n−1)/2. The first ten pentagonal numbers are:

1, 5, 12, 22, 35, 51, 70, 92, 117, 145, ...

It can be seen that P4 + P7 = 22 + 70 = 92 = P8. However,
their difference, 70 − 22 = 48, is not pentagonal.

Find the pair of pentagonal numbers, Pj and Pk, for which
their sum and difference are pentagonal and D = |Pk − Pj|
is minimised; what is the value of D?

Antwoord: 5,482,660
"""

def opdracht44(window = 10**7):
    gen = polygonizer(window, 3)
    next(gen)   # skip 0
    lpgs = list(gen)
    spgs = set(lpgs)
    for a in range(len(lpgs)):
        for b in range(a, len(lpgs)):
            if lpgs[a] + lpgs[b] in spgs and lpgs[b] - lpgs[a] in spgs:
                return lpgs[b] - lpgs[a]
    return 0

"""
#45: Triangular, pentagonal, and hexagonal

Triangle, pentagonal, and hexagonal numbers are generated by the following formulae:
Triangle     Tn=n(n+1)/2     1, 3, 6, 10, 15, ...
Pentagonal     Pn=n(3n−1)/2     1, 5, 12, 22, 35, ...
Hexagonal     Hn=n(2n−1)     1, 6, 15, 28, 45, ...

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

def opdracht45():
    spentagon = set(polygonizer(2*10**9, 3))
    for h in [hexagon(n) for n in range(144, 99999)]:
        if h in spentagon: return h
    return 0

"""
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
"""

def opdracht46():
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
    return 0

"""
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
"""

"""
https://blog.dreamshire.com/project-euler-47/
"""

def opdracht47(L = 3*10**5, nf = 4, ns = 4):
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
    return 0

"""
#48: Self powers

The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.

Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.

Antwoord: 9,110,846,700
"""

def opdracht48():
    return sum(x**x for x in range(1,1001)) % 10**10

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

def opdracht49():
    def sameDigs(a, b): return hasDigitsOnce(b, list(digits(a)))
    sp4 = {x for x in sieve(10**4) if x > 1487}
    for p in sp4:
        if p+3330 in sp4 and p+6660 in sp4 and sameDigs(p,p+3330) and sameDigs(p,p+6660):
            return (p + 6660) + (p + 3330) * 10**4 + p * 10**8
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

def opdracht50(limit = 10**6):
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

def opdracht51():
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
def opdracht52a():
    f = lambda n:sorted(str(n))
    n = 99999
    while not f(n*2) == f(n*3) == f(n*4) == f(n*5) == f(n*6): n += 9
    return n

def opdracht52():
    def test(n, r = range(2, 6+1)):
        nset = list(digits(n))
        for m in r:
            if hasDigitsOnce(n * m, list(nset)) == False:
                return False
        return True
    for n in range(2, 9**9):
        if test(n):
            return n
    return 0

"""
#53: Combinatoric selections

There are exactly ten ways of selecting three from five, 12345:

123, 124, 125, 134, 135, 145, 234, 235, 245, and 345

In combinatorics, we use the notation, 5C3 = 10.

In general,
nCr =  
n!
r!(n−r)!
 ,where r ≤ n, n! = n×(n−1)×...×3×2×1, and 0! = 1.

It is not until n = 23, that a value exceeds one-million: 23C10 = 1144066.

How many, not necessarily distinct, values of  nCr,
for 1 ≤ n ≤ 100, are greater than one-million?

Antwoord: 4,075
"""

def opdracht53(nlimit = 101, limit = 10**6):
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

def opdracht54():
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

def opdracht55(r = range(10000)):
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

def opdracht56():
    best = 0
    for a in range(100):
        for b in range(100):
            best = max(best, sum(digits(a**b)))
    return best

"""
#57: Square root convergents

It is possible to show that the square root of two
can be expressed as an infinite continued fraction.

√ 2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...

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

import math
def problem57a(N=1000):
  c = 0
  n = d = 1
  for k in range(N):
    n, d = 2 * d + n, d + n
    print("{}: log10({}) = {}, log10({}) = {}".format(k, n, math.log10(n), d, math.log10(d)))
    if int(math.log10(n)) > int(math.log10(d)):
      c+= 1
  return c

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
lying along both diagonals are prime; that is, a ratio of 8/13 ≈ 62%.

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

Each character on a computer is assigned a unique code and the preferred standard is ASCII (American Standard Code for Information Interchange). For example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.

A modern encryption method is to take a text file, convert the bytes to ASCII, then XOR each byte with a given value, taken from a secret key. The advantage with the XOR function is that using the same encryption key on the cipher text, restores the plain text; for example, 65 XOR 42 = 107, then 107 XOR 42 = 65.

For unbreakable encryption, the key is the same length as the plain text message, and the key is made up of random bytes. The user would keep the encrypted message and the encryption key in different locations, and without both "halves", it is impossible to decrypt the message.

Unfortunately, this method is impractical for most users, so the modified method is to use a password as a key. If the password is shorter than the message, which is likely, the key is repeated cyclically throughout the message. The balance for this method is using a sufficiently long password key for security, but short enough to be memorable.

Your task has been made easy, as the encryption key consists of three lower case characters. Using cipher.txt (right click and 'Save Link/Target As...'), a file containing the encrypted ASCII codes, and the knowledge that the plain text must contain common English words, decrypt the message and find the sum of the ASCII values in the original text.
"""

msg59 = [79, 59, 12, 2, 79, 35, 8, 28, 20, 2, 3, 68, 8, 9, 68, 45,
0, 12, 9, 67, 68, 4, 7, 5, 23, 27, 1, 21, 79, 85, 78, 79, 85, 71,
38, 10, 71, 27, 12, 2, 79, 6, 2, 8, 13, 9, 1, 13, 9, 8, 68, 19, 7,
1, 71, 56, 11, 21, 11, 68, 6, 3, 22, 2, 14, 0, 30, 79, 1, 31, 6, 23,
19, 10, 0, 73, 79, 44, 2, 79, 19, 6, 28, 68, 16, 6, 16, 15, 79, 35,
8, 11, 72, 71, 14, 10, 3, 79, 12, 2, 79, 19, 6, 28, 68, 32, 0, 0, 73,
79, 86, 71, 39, 1, 71, 24, 5, 20, 79, 13, 9, 79, 16, 15, 10, 68, 5, 10,
3, 14, 1, 10, 14, 1, 3, 71, 24, 13, 19, 7, 68, 32, 0, 0, 73, 79, 87, 71, 39, 1, 71, 12, 22, 2, 14, 16, 2, 11, 68, 2, 25, 1, 21, 22, 16, 15, 6, 10, 0, 79, 16, 15, 10, 22, 2, 79, 13, 20, 65, 68,
41, 0, 16, 15, 6, 10, 0, 79, 1, 31, 6, 23, 19, 28, 68, 19, 7, 5, 19, 79, 12, 2, 79, 0, 14, 11, 10, 64, 27, 68, 10, 14, 15, 2, 65, 68, 83, 79, 40, 14, 9, 1, 71, 6, 16, 20, 10, 8, 1, 79, 19, 6, 28, 68, 14, 1, 68, 15, 6, 9, 75, 79, 5, 9, 11, 68, 19, 7, 13, 20, 79, 8, 14, 9, 1, 71, 8, 13, 17, 10, 23, 71, 3, 13, 0, 7, 16, 71, 27, 11, 71, 10, 18, 2, 29,
29, 8, 1, 1, 73, 79, 81, 71, 59, 12, 2, 79, 8, 14, 8, 12, 19, 79, 23, 15, 6, 10, 2, 28, 68, 19, 7, 22, 8, 26, 3, 15, 79, 16, 15, 10, 68, 3, 14, 22, 12, 1, 1, 20, 28, 72, 71, 14, 10, 3, 79, 16, 15, 10, 68, 3, 14, 22, 12, 1, 1, 20, 28, 68, 4, 14, 10, 71, 1,
1, 17, 10, 22, 71, 10, 28, 19, 6, 10, 0, 26, 13, 20, 7, 68, 14, 27, 74, 71, 89, 68, 32, 0, 0, 71, 28, 1, 9, 27, 68, 45, 0, 12, 9, 79, 16, 15, 10, 68, 37, 14, 20, 19, 6, 23, 19, 79, 83, 71, 27, 11, 71, 27, 1, 11, 3, 68, 2, 25, 1, 21, 22, 11, 9, 10, 68, 6, 13, 11, 18, 27, 68, 19, 7, 1, 71, 3, 13, 0, 7, 16, 71, 28, 11, 71, 27, 12, 6, 27, 68,
2, 25, 1, 21, 22, 11, 9, 10, 68, 10, 6, 3, 15, 27, 68, 5, 10, 8, 14, 10, 18, 2, 79, 6, 2, 12, 5, 18, 28, 1, 71, 0, 2, 71, 7, 13, 20, 79, 16, 2, 28, 16, 14, 2, 11, 9, 22, 74, 71, 87, 68, 45, 0, 12, 9, 79, 12, 14, 2, 23, 2, 3, 2, 71, 24, 5, 20, 79, 10, 8, 27, 68, 19, 7, 1, 71, 3, 13, 0, 7, 16, 92, 79, 12, 2, 79, 19, 6, 28, 68, 8, 1, 8,
30, 79, 5, 71, 24, 13, 19, 1, 1, 20, 28, 68, 19, 0, 68, 19, 7, 1, 71,
3, 13, 0, 7, 16, 73, 79, 93, 71, 59, 12, 2, 79, 11, 9, 10, 68, 16, 7,
11, 71, 6, 23, 71, 27, 12, 2, 79, 16, 21, 26, 1, 71, 3, 13, 0, 7, 16,
75, 79, 19, 15, 0, 68, 0, 6, 18, 2, 28, 68, 11, 6, 3, 15, 27, 68, 19, 0, 68, 2, 25, 1, 21, 22, 11, 9, 10, 72, 71, 24, 5, 20, 79, 3, 8, 6, 10, 0, 79, 16, 8, 79, 7, 8, 2, 1, 71, 6, 10, 19, 0, 68, 19, 7, 1, 71, 24, 11, 21, 3, 0, 73, 79, 85, 87, 79, 38, 18, 27, 68, 6, 3, 16, 15, 0, 17, 0, 7, 68, 19, 7, 1, 71, 24, 11, 21, 3, 0, 71, 24, 5, 20, 79, 9, 6, 11, 1, 71, 27, 12, 21, 0, 17, 0, 7, 68, 15, 6, 9, 75, 79, 16, 15, 10, 68, 16, 0, 22, 11, 11, 68, 3, 6, 0, 9, 72, 16, 71, 29, 1, 4, 0, 3, 9, 6, 30, 2, 79, 12, 14, 2, 68, 16, 7, 1, 9, 79, 12, 2, 79, 7, 6, 2, 1, 73, 79, 85, 86, 79, 33, 17, 10, 10, 71, 6, 10, 71, 7, 13, 20, 79, 11, 16, 1, 68, 11, 14, 10, 3, 79, 5, 9, 11, 68, 6, 2, 11, 9, 8, 68, 15, 6, 23, 71, 0, 19, 9, 79, 20, 2, 0, 20, 11, 10, 72, 71, 7, 1, 71, 24, 5, 20, 79, 10, 8, 27, 68, 6, 12, 7, 2, 31, 16, 2, 11, 74, 71, 94, 86, 71, 45, 17, 19, 79, 16, 8, 79, 5, 11, 3, 68, 16, 7, 11, 71, 13, 1, 11, 6, 1, 17, 10, 0, 71, 7, 13, 10, 79, 5, 9, 11, 68, 6, 12, 7, 2, 31, 16, 2, 11, 68, 15, 6, 9, 75, 79, 12, 2, 79, 3, 6, 25, 1, 71, 27, 12, 2, 79, 22, 14, 8, 12, 19, 79, 16, 8, 79, 6, 2, 12, 11, 10, 10, 68, 4, 7, 13, 11, 11, 22, 2, 1, 68, 8, 9, 68, 32, 0, 0, 73, 79, 85, 84, 79, 48, 15, 10, 29, 71, 14, 22, 2, 79, 22, 2, 13, 11, 21, 1, 69, 71, 59, 12, 14, 28, 68, 14, 28, 68, 9, 0, 16, 71, 14, 68, 23, 7, 29, 20, 6, 7, 6, 3, 68, 5, 6, 22, 19, 7, 68, 21, 10, 23, 18, 3, 16, 14, 1, 3, 71, 9, 22, 8, 2, 68, 15, 26, 9, 6, 1, 68, 23, 14, 23, 20, 6, 11, 9, 79, 11, 21, 79, 20, 11, 14, 10, 75, 79, 16, 15, 6, 23, 71, 29, 1, 5, 6, 22, 19, 7, 68, 4, 0, 9, 2, 28, 68, 1, 29, 11, 10, 79, 35, 8, 11, 74, 86, 91, 68, 52, 0, 68, 19, 7, 1, 71, 56, 11, 21, 11, 68, 5, 10, 7, 6, 2, 1, 71, 7, 17, 10, 14, 10, 71, 14, 10, 3, 79, 8, 14, 25, 1, 3, 79, 12, 2, 29, 1, 71, 0, 10, 71, 10, 5, 21, 27, 12, 71, 14, 9, 8, 1, 3, 71, 26, 23, 73, 79, 44, 2, 79, 19, 6, 28, 68, 1, 26, 8, 11,
79, 11, 1, 79, 17, 9, 9, 5, 14, 3, 13, 9, 8, 68, 11, 0, 18, 2, 79, 5, 9, 11, 68, 1, 14, 13, 19, 7, 2, 18, 3, 10, 2, 28, 23, 73, 79, 37, 9, 11, 68, 16, 10, 68, 15, 14, 18, 2, 79, 23, 2, 10, 10, 71, 7, 13,
20, 79, 3, 11, 0, 22, 30, 67, 68, 19, 7, 1, 71, 8, 8, 8, 29, 29, 71,
0, 2, 71, 27, 12, 2, 79, 11, 9, 3, 29, 71, 60, 11, 9, 79, 11, 1, 79, 16, 15, 10, 68, 33, 14, 16, 15, 10, 22, 73]

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

def problem59():
    best_sumdif = 999999.9
    best_key = (0, 0, 0)
    for key in keygen(97, 123):
        s = "".join(decipher(msg59, key))
        letters, absolute, relative = analysis(s)
        sumdif = compare(relative, english)
        if sumdif < best_sumdif:
            best_sumdif = sumdif
            best_key = key
    s = "".join(decipher(msg59, best_key))
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

Triangle, square, pentagonal, hexagonal, heptagonal, and octagonal numbers are all figurate (polygonal) numbers and are generated by the following formulae:
Triangle     P3,n=n(n+1)/2     1, 3, 6, 10, 15, ...
Square     P4,n=n2     1, 4, 9, 16, 25, ...
Pentagonal     P5,n=n(3n−1)/2     1, 5, 12, 22, 35, ...
Hexagonal     P6,n=n(2n−1)     1, 6, 15, 28, 45, ...
Heptagonal     P7,n=n(5n−3)/2     1, 7, 18, 34, 55, ...
Octagonal     P8,n=n(3n−2)     1, 8, 21, 40, 65, ...

The ordered set of three 4-digit numbers: 8128, 2882, 8281, has three interesting properties.

1.    The set is cyclic, in that the last two digits of each number is the first two digits of the next number (including the last number with the first).
2.    Each polygonal type: triangle (P3,127=8128), square (P4,91=8281), and pentagonal (P5,44=2882), is represented by a different number in the set.
3.    This is the only set of 4-digit numbers with this property.

Find the sum of the only ordered set of six cyclic 4-digit numbers for which each polygonal type: triangle, square, pentagonal, hexagonal, heptagonal, and octagonal, is represented by a different number in the set.
"""


"""
Einde opdrachten
"""

def runn2(n = 1):
    if n == 1: return opdracht1()
    if n == 2: return opdracht2()
    if n == 3: return opdracht3()
    if n == 4: return opdracht4()
    if n == 5: return opdracht5()
    if n == 6: return opdracht6()
    if n == 7: return opdracht7()
    if n == 8: return opdracht8()
    if n == 9: return opdracht9()
    if n == 10: return opdracht10()
    if n == 11: return opdracht11()
    if n == 12: return opdracht12()
    if n == 13: return opdracht13()
    if n == 14: return opdracht14()
    if n == 15: return opdracht15()
    if n == 16: return opdracht16()
    if n == 17: return opdracht17()
    if n == 18: return opdracht18()
    if n == 19: return opdracht19()
    if n == 20: return opdracht20()
    if n == 21: return opdracht21()
    if n == 22: return opdracht22()
    if n == 23: return opdracht23()
    if n == 24: return opdracht24()
    if n == 25: return opdracht25()
    if n == 26: return opdracht26()
    if n == 27: return opdracht27()
    if n == 28: return opdracht28()
    if n == 29: return opdracht29()
    if n == 30: return opdracht30()
    if n == 31: return opdracht31()
    if n == 32: return opdracht32()
    if n == 33: return opdracht33()
    if n == 34: return opdracht34()
    if n == 35: return opdracht35()
    if n == 36: return opdracht36()
    if n == 37: return opdracht37()
    if n == 38: return opdracht38()
    if n == 39: return opdracht39()
    if n == 40: return opdracht40()
    if n == 41: return opdracht41()
    if n == 42: return opdracht42()
    if n == 43: return opdracht43()
    if n == 44: return opdracht44()
    if n == 45: return opdracht45()
    if n == 46: return opdracht46()
    if n == 47: return opdracht47()
    if n == 48: return opdracht48()
    if n == 49: return opdracht49()
    if n == 50: return opdracht50()
    if n == 51: return opdracht51()
    if n == 52: return opdracht52()
    if n == 53: return opdracht53()
    if n == 54: return opdracht54()
    if n == 55: return opdracht55()
    if n == 56: return opdracht56()
    if n == 57: return problem57()
    if n == 58: return problem58()
    if n == 59: return problem59()
    if n == 60: return problem60()
    return 0

answers = [233168, 4613732, 6857, 906609, 232792560, 25164150, 104743, 23514624000,
    31875000, 142913828922, 70600674, 76576500, 5537376230, 837799, 137846528820,
    1366, 21124, 1074, 171, 648, 31626, 871198282, 4179871, 2783915460, 4782, 983,
    -59231, 669171001, 9183, 443839, 73682, 45228, 100, 40730, 55, 872187, 748317,
    932718654, 840, 210, 7652413, 162, 16695334890, 5482660, 1533776805, 5777,
    134043, 9110846700, 296962999629, 997651, 121313, 142857, 4075, 376, 249, 972,
    153, 26241, 107359, 26033, 28684]

#answers[51 - 1] = 0

import time
import math
import concurrent.futures

def runjob(n):
    ts = time.time()
    ret = runn2(n)
    assert ret == answers[n - 1]
    print("#{}: {} {}s".format(n, ret, math.floor(time.time() - ts)))

def runm(l = list(range(1, 60 + 1))):
    ts = time.time()
    with concurrent.futures.ProcessPoolExecutor() as executor:
        executor.map(runjob, l)
    print("Total: {}s".format(math.floor(time.time() - ts)))

def runs(l = range(1, 60 + 1)):
    ts = time.time()
    for job in l:
        runjob(job)
    print("Total: {}s".format(math.floor(time.time() - ts)))


