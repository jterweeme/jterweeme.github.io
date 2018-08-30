using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

class Sieve
{
    private bool[] _sieve;
    private List<UInt32> _primes;
    private int _pos = 0;

    public Sieve(int limit)
    {
        _sieve = new bool[limit];
        _primes = new List<UInt32>();
        for (UInt32 i = 0; i < limit; i++) _sieve[i] = true;
        _sieve[0] = _sieve[1] = false;
        for (UInt32 p = 2; p * p < limit; p++)
            if (_sieve[p] == true)
                for (UInt32 i = p * 2; i < limit; i += p)
                    _sieve[i] = false;
        for (UInt32 i = 0; i < limit; i++)
            if (_sieve[i]) _primes.Add(i);
    }
    
    public bool hasNext()
    {   return _pos < _primes.Count;
    }

    public UInt32 next()
    {   return _primes[_pos++];
    }
}

class PrimeFactors
{
    List<UInt32> _lprimes;
    UInt64 _n;

    public PrimeFactors(List<UInt32> lprimes, UInt64 n)
    {
        _lprimes = lprimes;
        _n = n;
    }

    public bool hasNext() { return _n > 1; }
    
    public UInt64 next()
    {
        UInt64 factor = 0;
        foreach (var prime in _lprimes)
        {
            if (_n % prime == 0)
            {
                factor = prime;
                break;
            }
        }
        _n = _n / factor;
        return factor;
    }
}

class Euler
{

/*
#1 Multiples of 3 and 5

If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

Antwoord: 233,168
*/
    static UInt32 summation(UInt32 n, UInt32 max)
    {   UInt32 len = max / n;
        return ((len * (len + 1)) >> 1) * n;
    }

    static UInt32 problem1()
    {   return summation(3, 999) + summation(5, 999) - summation(15, 999);
    }

/*
#2 Even Fibonacci numbers

Each new term in the Fibonacci sequence is generated by adding the previous
two terms. By starting with 1 and 2, the first 10 terms will be:

1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

By considering the terms in the Fibonacci sequence whose values do not
exceed four million, find the sum of the even-valued terms.

Antwoord: 4,613,732
*/

    static UInt32 problem2(UInt32 max = 4000000)
    {   UInt32 term1 = 1, term2 = 2, temp = 0, sum = 2;
        while (true)
        {
            temp = term1 + term2;
            if (temp > max) break;
            if (temp % 2 == 0) sum += temp;
            term1 = term2;
            term2 = temp;
        }
        return sum;
    }

/*
#3 Largest prime factor

The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
*/

    static UInt64 problem3(UInt64 n = 600851475143)
    {   Sieve sieve = new Sieve(9999);
        List<UInt32> lprimes = new List<UInt32>();
        while (sieve.hasNext())
            lprimes.Add(sieve.next());
        PrimeFactors pf = new PrimeFactors(lprimes, n);
        UInt64 xmax = 0;
        while (pf.hasNext())
            xmax = Math.Max(xmax, pf.next());
        return xmax;
    }

/*
#4 Largest palindrome product

A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 x 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
*/

    static UInt32 reverse(UInt32 n, byte xbase = 10)
    {   UInt32 rev = 0;
        for (UInt32 temp = n; temp != 0; temp /= xbase)
            rev = rev * xbase + temp % xbase;
        return rev;
    }

    static bool ispalindrome(UInt32 n, byte xbase = 10)
    {   return n == reverse(n, xbase);
    }

    static UInt32 problem4()
    {
        UInt32 best = 0;
        for (UInt32 a = 0; a < 1000; a++)
        {   for (UInt32 b = 0; b < 1000; b++)
            {   UInt32 c = a * b;
                if (ispalindrome(c) && c > best)
                    best = c;
            }
        }
        return best;
    }

/*
#5 Smallest multiple

2520 is the smallest number that can be divided by
each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly
divisible by all of the numbers from 1 to 20?

Antwoord: 232,792,560
*/

    static bool isdivisible(UInt32 n, UInt32 lower, UInt32 max)
    {   for (UInt32 i = lower; i <= max; i++) if (n % i > 0) return false;
        return true;
    }

    static UInt32 problem5(UInt32 lower = 11, UInt32 max = 20)
    {   UInt32 start = 2520, number = start;
        while (isdivisible(number, lower, max) == false)
            number += start;
        return number;
    }

/*
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
*/

    static UInt32 problem6(UInt32 min = 1, UInt32 max = 100)
    {   UInt32 sumsquare = 0, squaresum = 0;
        for (UInt32 i = min; i <= max; i++) sumsquare += i * i;
        for (UInt32 i = min; i <= max; i++) squaresum += i;
        squaresum = squaresum * squaresum;
        return squaresum - sumsquare;
    }

/*
#7 By listing the first six prime numbers: 2, 3, 5,
7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?

Antwoord: 104,743
*/

    static UInt32 problem7(UInt32 n = 10001)
    {
        Sieve sieve = new Sieve(999999);
        UInt32 ret = 0;
        for (UInt32 i = 0; i < n; i++)
            ret = sieve.next();
        return ret;
    }

/*
#8 Largest product in a series

The four adjacent digits in the 1000-digit number that
have the greatest product are 9 × 9 × 8 × 9 = 5832.

Find the thirteen adjacent digits in the 1000-digit number that
have the greatest product. What is the value of this product?

Antwoord: 23,514,624,000
*/

    const string series8 = "73167176531330624919225119674426574742355349194934" +
        "96983520312774506326239578318016984801869478851843" +
        "85861560789112949495459501737958331952853208805511" +
        "12540698747158523863050715693290963295227443043557" +
        "66896648950445244523161731856403098711121722383113" +
        "62229893423380308135336276614282806444486645238749" +
        "30358907296290491560440772390713810515859307960866" +
        "70172427121883998797908792274921901699720888093776" +
        "65727333001053367881220235421809751254540594752243" +
        "52584907711670556013604839586446706324415722155397" +
        "53697817977846174064955149290862569321978468622482" +
        "83972241375657056057490261407972968652414535100474" +
        "82166370484403199890008895243450658541227588666881" +
        "16427171479924442928230863465674813919123162824586" +
        "17866458359124566529476545682848912883142607690042" +
        "24219022671055626321111109370544217506941658960408" +
        "07198403850962455444362981230987879927244284909188" +
        "84580156166097919133875499200524063689912560717606" +
        "05886116467109405077541002256983155200055935729725" +
        "71636269561882670428252483600823257530420752963450";

    static UInt64 problem8(string s = series8)
    {
        return 0;
#if false
        UInt64 cur = 0, best = 0;
        
        foreach (char c in s)
        {
            cur = (cur % 1000000000000) * 10 + Convert.ToUInt64(c);
            UInt64 peel = cur;
            UInt64 product = 1;
            while (peel > 0)
            {
                product *= peel % 10;
                peel = peel / 10;
            }
            if (product > best) best = product;
        }
        return best;
#endif
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

    static UInt32 problem9(UInt32 search = 1000)
    {   for (UInt32 a = 1; a <= search - 2; a++)
        {   for (UInt32 b = 1; b <= search - 2; b++)
            {   UInt32 c = search - a - b;
                if (a * a + b * b == c * c)
                    return a * b * c;
            }
        }
        return 0;
    }

/*
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
*/

    static UInt64 problem10(int limit = 2000000)
    {   Sieve sieve = new Sieve(limit);
        UInt64 sum = 0;
        while (sieve.hasNext())
            sum += sieve.next();
        return sum;
    }

/*
#11 Largest product in a grid

In the 20x20 grid below, four numbers along
a diagonal line have been marked in red.

The product of these numbers is 26 x 63 x 78 x 14 = 1788696.

What is the greatest product of four adjacent numbers in the same
direction (up, down, left, right, or diagonally) in the 20x20 grid?

Antwoord: 70,600,674
*/

static UInt32[,] t11 = new UInt32[20,20]
            {{ 8, 2,22,97,38,15, 0,40, 0,75, 4, 5, 7,78,52,12,50,77,91, 8},
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

static UInt32 problem11()
{
    UInt32 best = 0;
    for (byte i = 0; i < 20; i++)
    {
        for (byte j = 0; j < 16; j++)
        {
            UInt32 prod = t11[i,j] * t11[i,j+1] * t11[i,j+2] * t11[i,j+3];
            if (prod > best) best = prod;
            prod = t11[j, i] * t11[j + 1,i] * t11[j+2,i] * t11[j+3,i];
            if (prod > best) best = prod;
        }
    }
    for (byte i = 0; i < 16; i++)
    {
        for (byte j = 0; j < 16; j++)
        {
            UInt32 prod = t11[i,j] * t11[i+1,j+1] * t11[i+2,j+2] * t11[i+3,j+3];
            if (prod > best) best = prod;
        }
    }
    for (byte i = 3; i < 20; i++)
    {   for (byte j = 0; j < 16; j++)
        {   UInt32 prod = t11[i,j] * t11[i - 1,j+1] * t11[i-2,j+2] * t11[i-3,j+3];
            if (prod > best) best = prod;
        }
    }
    return best;
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

/*
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

Antwoord: 5,537,376,230
*/

/*
#15 Lattice paths

Starting in the top left corner of a 2x2 grid, and only being able to move
to the right and down, there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20x20 grid?

Antwoord: 137,846,528,820
*/

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

    static UInt64 problem25()
    {
        return 0;
#if false
        int i = 0;
        int cnt = 2;
        BigInteger limit = BigInteger.Pow(10, 999);
        BigInteger[] fib = new BigInteger[3];
         
        fib[0] = 1;
        fib[2] = 1;
         
        while (fib[i] <= limit) {
            i = (i + 1) % 3;
            cnt++;
            fib[i] = fib[(i + 1) % 3] + fib[(i + 2) % 3];
        }
        return i;
#endif
    }

/*
#39: Integer right triangles

If p is the perimeter of a right angle triangle with integral length
sides, {a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p <= 1000, is the number of solutions maximised?

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

    static UInt32 problem39()
    {   UInt32 best_p = 0, best_solutions = 0;
        for (UInt32 p = 100; p <= 1000; p += 2)
        {   UInt32 solutions = 0;
            for (UInt32 a = 2; a < p / 3; a++)
                solutions += (p * (p - 2 * a) % (2 * (p - a)) == 0) ? (UInt32)1 : (UInt32)0;
            if (solutions > best_solutions)
            {   best_solutions = solutions;
                best_p = p;
            }
        }
        return best_p;
    }

/*
#62: Cubic permutations

The cube, 41063625 (345^3), can be permuted to produce two other
cubes: 56623104 (384^3) and 66430125 (405^3). In fact, 41063625 is
the smallest cube which has exactly three permutations of its
digits which are also cube.

Find the smallest cube for which exactly
five permutations of its digits are cube.

Antwoord: 127,035,954,683
*/

    class Cube
    {
        public long N { get; set; }
        public int Perms { get; set; }
    }

    public static long makeSmallestPerm(long n)
    {
        long k = n;
        int[] digits = new int[10];
        long retVal = 0;

        while (k > 0) {
            digits[k % 10]++;
            k /= 10;
        }

        for (int i = 9; i >= 0; i--)
            for (int j = 0; j < digits[i]; j++)
                retVal = retVal * 10 + i;
        return retVal;
    }

    public static long problem62() {
        Cube result = null;
        long n = 345;
        bool found = false;
        SortedList<long, Cube> cubes = new SortedList<long, Cube>();

        while (!found)
        {
            n++;
            long smallestPerm = makeSmallestPerm(n*n*n);
            if (!cubes.ContainsKey(smallestPerm)) {
                cubes.Add(smallestPerm, new Cube {N=n, Perms = 0});
            }

            if (++cubes[smallestPerm].Perms == 5) {
                found = true;
                result = cubes[smallestPerm];
            }
        }
        return result.N*result.N*result.N;
    }


/*
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
*/

    static public int Astar(int[,] grid, int minval)
    {
        int root = grid.GetLength(0);
        int[,] g = new int[root, root];
        int[,] h = new int[root, root];
        int[,] searched = new int[root, root];

        SortedList<Tuple<int,int>, Tuple<int, int>> openList;
        openList = new SortedList<Tuple<int,int>, Tuple<int, int>>();

        for (int i = 0; i < root; i++)
        {   for (int j = 0; j < root; j++) {
                h[i, j] = minval*(2*(root - 1)+1-i-j);
                g[i, j] = int.MaxValue;
            }
        }

        //Add the start square
        g[0,0] = grid[0,0];
        openList.Add(new Tuple<int,int>(g[0, 0] + h[0, 0],0), new Tuple<int, int>(0, 0));

        while (searched[root-1, root-1] < 2)
        {
            Tuple<int, int> current = openList.ElementAt(0).Value;
            openList.RemoveAt(0);
            int ci = current.Item1;
            int cj = current.Item2;
            searched[current.Item1, current.Item2] = 2;

            //Check the four adjacent squares
            for (int k = 0; k < 4; k++)
            {   int cinew = 0, cjnew = 0;
                switch (k)
                {
                    case 0: //Check the square above
                        cinew = ci - 1;
                        cjnew = cj;
                        break;
                    case 1: //Check the square below
                        cinew = ci + 1;
                        cjnew = cj;
                        break;
                    case 2: //Check the square right
                        cinew = ci;
                        cjnew = cj+1;
                        break;
                    case 3: //Check the square left
                        cinew = ci;
                        cjnew = cj -1;
                        break;
                }
                if (cinew >= 0 && cinew < root && cjnew >= 0 && cjnew < root &&
                    searched[cinew, cjnew] < 2)
                {
                    if (g[cinew, cjnew] > g[ci, cj] + grid[cinew, cjnew])
                    {   g[cinew, cjnew] = g[ci, cj] + grid[cinew, cjnew];
                        if(searched[cinew, cjnew] == 1)
                        {   int index = openList.IndexOfValue(new Tuple<int, int>(cinew, cjnew));
                            openList.RemoveAt(index);
                        }
                        int l = 0;
                        while (true)
                        {   Tuple<int,int> tpl;
                            tpl = new Tuple<int,int>(g[cinew, cjnew] + h[cinew, cjnew],l);
                            if (openList.ContainsKey(tpl) == false) break;
                            l++;
                        }
                        Tuple<int,int> tpl2;
                        tpl2 = new Tuple<int,int>(g[cinew, cjnew] + h[cinew, cjnew],l);
                        openList.Add(tpl2, new Tuple<int, int>(cinew, cjnew));
                        searched[cinew, cjnew] = 1;
                    }
                }
            }
        } 
        return g[root-1, root-1];
    }

    static int problem83(string fn = "euler83.txt")
    {
        int [,] grid;
        int lines = 0;
        string line;
        string[] linePieces;
        int minval = int.MaxValue;
        StreamReader r = new StreamReader(fn);
        while (r.ReadLine() != null) lines++;
        grid = new int[lines, lines];
        r.BaseStream.Seek(0, SeekOrigin.Begin);
        for (int j = 0; (line = r.ReadLine()) != null; j++)
        {   linePieces = line.Split(' ');
            for (int i = 0; i < linePieces.Length; i++)
            {   grid[j, i] = int.Parse(linePieces[i]);
                minval = (minval > grid[j, i]) ? grid[j, i] : minval;
            }
        }
        r.Close();
        return Astar(grid, minval);
    }

/*
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
*/

    static Random random;
    static int cPos = 0;
    static int ccPos = 0;
    static int chancePos = 0;

    static private void CC()
    {   int[] cc = { 0, 10 };
        ccPos = ++ccPos % 16;
        if (ccPos < 2) cPos = cc[ccPos];
        return;
    }

    static private void chance()
    {
        int[] chance = { 0, 10, 11, 24, 39, 5 };
        chancePos = ++chancePos % 16;
        if (chancePos < 6) cPos = chance[chancePos];

        if (chancePos == 6 || chancePos == 7) {
            if (cPos == 7) cPos = 15;
            if (cPos == 22) cPos = 25;
            if (cPos == 36) cPos = 5;
        }

        if (chancePos == 8) cPos = (cPos == 22) ? 28 : 12;
        if (chancePos == 9) cPos -= 3;
        return;
    }

    static public int problem84()
    {   
        int[] board = new int[40];
        int samples = 1000000;
        random = new Random();
        int doubles = 0;

        for (int i = 0; i < samples; i++) {
            //roll the dices
            int dice1 = random.Next(4) + 1;
            int dice2 = random.Next(4) + 1;
            //Check doubles
            doubles = (dice1 == dice2) ? doubles + 1 : 0;
            if (doubles > 2)
            {   cPos = 10;
                doubles = 0;
            } else {
                //Move to the square
                cPos = (cPos + dice1 + dice2) % 40;
                //Handle chance
                //Important first, as you can go CH3->CC3
                if (cPos == 7 || cPos == 22 || cPos == 36) chance();
                //Handle CH
                if (cPos == 2 || cPos == 17 || cPos == 33) CC();
                //Handle G2J
                if (cPos == 30) cPos = 10;
            }
            board[cPos]++;
        }

        int[] index = board
                        .Select((item, indx) => new { Item = item, Index = indx })
                        .OrderByDescending(x => x.Item)
                        .Select(x => x.Index)
                        .ToArray();

        string modalstring = "";
        for (int i = 0; i < 3; i++) {
            if (index[i] < 10) modalstring += "0";
            modalstring += index[i].ToString();
        }

        return Int32.Parse(modalstring);
    }

    static UInt64[] answers = {233168, 4613732, 6857, 906609, 232792560,
        25164150, 104743, 0, 31875000, 142913828922, 70600674, 76576500,
        5537376230, 837799, 137846528820};

    static UInt64 run(UInt32 p)
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
        }
        return 0;
    }

    static void runjob(UInt32 p)
    {
        UInt64 answer = run(p);

        if (answer != answers[p - 1])
            throw new System.Exception("error");

        Console.WriteLine("#" + p + ": " + answer);
    }

    static void Main()
    {
        for (UInt32 i = 1; i <= 11; i++)
            runjob(i);
        Console.WriteLine(problem25());
        Console.WriteLine(problem39());
        Console.WriteLine(problem62());
        Console.WriteLine(problem83());
        Console.WriteLine(problem84());
    }
}

