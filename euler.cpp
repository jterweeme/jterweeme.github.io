//#define MULTITHREAD

#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <algorithm>
#include <vector>
#ifdef MULTITHREAD
#include <queue>
#include <functional>
#include <future>
#endif
using namespace std;

size_t strlen(const char *s)
{   size_t i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

void xmemcpy(void *dest, void *src, size_t n)
{   char *csrc = (char *)src;
    char *cdest = (char *)dest;
    size_t i;
    for (i = 0; i<n; i++) cdest[i] = csrc[i];
}

void *memset(void *s, int c, size_t n)
{   uint8_t *p = (uint8_t *)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}

template <typename T> void xswap(T *a, T *b)
{
    T c = *a; *a=*b; *b=c;
}

template <typename T> static T myPow(T base, T e)
{   if (e == 0) return 1;
    T ret = base;
    while (--e) ret *= base;
    return ret;
}

static uint32_t fac32(uint32_t n)
{   uint32_t product = 1;
    while (n > 1) product *= n--;
    return product;
}

template <typename T> static uint8_t decimals(T n)
{   uint8_t i = 0;
    while (n) n = n / 10, i++;
    return i;
}

template <typename T> static T digit(T n, T i)
{   return n / myPow<T>(10, i) % 10;
}

static uint32_t primeHelper(uint32_t n)
{   uint16_t presets[] = {3000,300,100,8};
    for (uint8_t i = 0; i < 4; i++)
        if (n > presets[i] * presets[i]) return n / presets[i];
    return n;
}

static bool isprime27(uint32_t n)
{   if (n == 0 || n == 1) return false;
    uint32_t limit = primeHelper(n);
    for (uint32_t i = 2; i < limit; i++)
        if (n % i == 0) return false;
    return true;
}

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t modulo)
{
    if (a <= 0xFFFFFFF && b <= 0xFFFFFFF)
        return (a * b) % modulo;
    uint64_t result = 0;
    uint64_t factor = a % modulo;
    while (b > 0)
    {
        if (b & 1)
        {
            result += factor;
            if (result >= modulo)
                result %= modulo;
        }

        factor <<= 1;
        if (factor >= modulo)
            factor %= modulo;

        b >>= 1;
    }
    return result;
}

static uint64_t powmod(uint64_t base, unsigned long long exponent, unsigned long long modulo)
{   uint64_t result = 1;
    while (exponent > 0)
    {   if (exponent & 1) result = mulmod(result, base, modulo);
        base = mulmod(base, base, modulo);
        exponent >>= 1;
    }
    return result;
}

// some code from      https://ronzii.wordpress.com/2012/03/04/miller-rabin-primality-test/
// with optimizations from    http://ceur-ws.org/Vol-1326/020-Forisek.pdf
// good bases can be found at http://miller-rabin.appspot.com/

static bool isPrime(uint64_t p)
{   const uint32_t bitmaskPrimes2to31 = (1 <<  2) | (1 <<  3) | (1 <<  5) | (1 <<  7) |
                                        (1 << 11) | (1 << 13) | (1 << 17) | (1 << 19) |
                                        (1 << 23) | (1 << 29); // = 0x208A28Ac
    if (p < 31) return (bitmaskPrimes2to31 & (1 << p)) != 0;
    if (p %  2 == 0 || p %  3 == 0 || p %  5 == 0 || p % 7 == 0 || 
        p % 11 == 0 || p % 13 == 0 || p % 17 == 0)
        return false;
    if (p < 17*19) return true;
    const uint32_t TestAgainst1[] = { 377687, 0 };
    const uint32_t TestAgainst2[] = { 31, 73, 0 };
    const uint32_t TestAgainst3[] = { 2, 7, 61, 0 };
    const uint32_t TestAgainst4[] = { 2, 13, 23, 1662803, 0 };
    const uint32_t TestAgainst7[] = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022, 0 };
    const uint32_t *testAgainst = TestAgainst7;
    if (p < 5329) testAgainst = TestAgainst1;
    else if (p < 9080191) testAgainst = TestAgainst2;
    else if (p < 4759123141ULL) testAgainst = TestAgainst3;
    else if (p < 1122004669633ULL) testAgainst = TestAgainst4;
    uint64_t d = p - 1;
    d >>= 1;
    uint32_t shift = 0;
    while ((d & 1) == 0) shift++, d >>= 1;
    do
    {   uint64_t x = powmod(*testAgainst++, d, p);
        if (x == 1 || x == p - 1) continue;
        bool maybePrime = false;
        for (uint32_t r = 0; r < shift; r++)
        {   x = powmod(x, 2, p);
            if (x == 1) return false;
            if (x == p - 1)
            {
               maybePrime = true;
               break;
            }
        }
        if (!maybePrime)
            return false;
    } while (*testAgainst != 0);
    return true;
}

template <typename T> class Generator
{
public:
    virtual bool hasNext() = 0;
    virtual T next() = 0;
};

static uint64_t max(Generator<uint64_t> &p)
{   uint64_t best = 0;
    while (p.hasNext()) best = max(p.next(), best);
    return best;
}

static uint64_t sum(Generator<uint32_t> &s)
{   uint64_t xsum = 0;
    while (s.hasNext()) xsum += s.next();
    return xsum;
}

class Sieve : public Generator<uint32_t>
{
private:
    uint8_t *_sieve;
    uint32_t _limit;
    uint32_t _next;
    bool _hasNext;
    vector<uint32_t> _primes;
    vector<uint32_t>::iterator _it;
public:
    Sieve(uint32_t limit);
    ~Sieve() { delete[] _sieve; }
    bool hasNext() { return _it != _primes.end(); }
    uint32_t next() { return *_it++; }
};

Sieve::Sieve(uint32_t limit)
{   _limit = limit;
    _sieve = new uint8_t[limit];
    for (uint32_t i = 0; i < limit; i++) _sieve[i] = 1;
    _sieve[0] = _sieve[1] = 0;
    for (uint32_t p = 2; p * p < limit; p++)
        if (_sieve[p] == 1)
            for (uint32_t i = p * 2; i < limit; i += p)
                _sieve[i] = 0;
    for (uint32_t i = 0; i < limit; i++)
        if (_sieve[i]) { _primes.push_back(i); }
    _it = _primes.begin();
}

static void testPrimes() __attribute__((unused));
static void testPrimes()
{
    Sieve sieve(99999);
    vector<uint32_t> primes;
    vector<bool> primes2;
    for (uint32_t i = 0; sieve.hasNext();)
    {   uint32_t prime = sieve.next();
        for (; i < prime; i++)
            primes2.push_back(false);
        primes2.push_back(true);
        i++;
    }
    for (uint32_t i = 0; i < primes2.size(); i++)
    {   if (isprime27(i) != primes2.at(i))
            cout << "ERROR\r\n";
    }
}

static const uint32_t triangle32(uint32_t n) { return n * (n + 1) >> 1; }
static const uint32_t pentagon32(uint32_t n) { return n * (3 * n - 1) / 2; }
static const uint32_t hexagon32(uint32_t n) { return n * (2 * n - 1); }

template <typename Inp, typename T>
uint32_t linSearch(Inp first, Inp last, const T& val)
{   uint32_t pos = 0;
    while (first != last)
    {   pos++;
        if (*first++ == val) return pos;
    }
    return 0;
}

template <typename Inp, typename T>
bool binSearch(Inp first, Inp last, const T& n)
{
    Inp middle = (last - first) / 2 + first;
    while (first <= last)
    {   if (*middle < n) first = middle + 1;
        else if (*middle == n) return true;
        else last = middle - 1;
        middle = (last - first) / 2 + first;
    }
    return false;
}

template <typename T> void bubbleSort(T begin, T end)
{   for (T a = begin; a < end - 1; a++)
        for (T b = begin; b < end - 1; b++)
            if (b[0] > b[1])
                xswap(b, b + 1);
}

template <typename T> bool hasDigitsOnce(T n, vector<uint8_t> &nset)
{   while (n)
    {
        uint32_t pos = linSearch(nset.begin(), nset.end(), n % 10);
        if (pos) nset.erase(nset.begin() + (pos - 1)); else return false;
        n = n / 10;
    }
    return true;
}

template <typename T> bool sameDigs(T a, T b)
{   vector<uint8_t> nset;
    for (;a ;a = a / 10) nset.push_back(a % 10);
    return hasDigitsOnce<T>(b, nset);
}

template <typename T> static string twostring(T n)
{   if (n == 0) return string("0");
    char buf[50];
    uint8_t len = decimals<T>(n), i = 0;
    for (i = 0; i < len; i++) buf[i] = digit<T>(n, len - (i + 1)) + '0';
    buf[i] = 0;
    return string(buf);
}

static string to_string32s(int32_t n)
{   char tmp[50];
    snprintf(tmp, 50, "%d", n);
    return string(tmp);
}

class LongNumber25
{
public:
    uint16_t _buf[1500];
public:
    void clear() { set(0); }
    LongNumber25() { clear(); }
    LongNumber25(uint64_t n) { set(n); }
    uint8_t decimal(uint16_t i) const { return _buf[i]; }
    void add(LongNumber25 &n);
    void operator+=(LongNumber25 n) { add(n); }
    void set(uint64_t n);
    void operator=(uint64_t n) { set(n); }
    void set(LongNumber25 n);
    void dump(ostream &os) const;
    bool equals(LongNumber25 n);
    uint16_t digits() const;
    LongNumber25 reverse();
    bool ispalindrome();
    void mul(uint8_t n);
};

bool LongNumber25::equals(LongNumber25 n)
{
    for (uint16_t i = 0; i < 1500; i++)
        if (_buf[i] != n._buf[i])
            return false;
    return true;
}

LongNumber25 LongNumber25::reverse()
{   LongNumber25 a;
    for (uint16_t i = digits(), j = 0; i > 0; i--, j++)
        a._buf[j] = _buf[i - 1];
    return a;
}

bool LongNumber25::ispalindrome()
{   return equals(reverse());
}

void LongNumber25::set(LongNumber25 n)
{   for (uint16_t i = 0; i < 1500; i++)
        _buf[i] = n.decimal(i);
}

void LongNumber25::dump(ostream &os) const
{   for (uint16_t i = digits(); i > 0; i--)
        os << (uint16_t)_buf[i - 1];
}

uint16_t LongNumber25::digits() const
{   uint16_t i;
    for (i = 1500; i > 0; i--)
        if (_buf[i - 1] > 0)
            return i;
    return 1;
}

void LongNumber25::set(uint64_t n)
{   memset(_buf, 0, sizeof(_buf));
    for (uint16_t i = 0; n > 0; i++)
        _buf[i] = n % 10, n = n / 10;
}

void LongNumber25::add(LongNumber25 &n)
{   uint8_t carry = 0;
    for (uint16_t i = 0; i < 1500; i++)
    {   _buf[i] += carry + n.decimal(i);
        carry = _buf[i] / 10;
        _buf[i] = _buf[i] % 10;
    }
}

void LongNumber25::mul(uint8_t n)
{
    uint8_t carry = 0;
    for (uint16_t i = 0; i < 1500; i++)
    {
        _buf[i] *= n;
        _buf[i] += carry;
        carry = _buf[i] / 10;
        _buf[i] = _buf[i] % 10;
    }
}

template <typename T> static T reverse(T n, uint8_t base = 10)
{   T rev = 0;
    for (T temp = n; temp != 0; temp /= base) rev = rev * base + temp % base;
    return rev;
}

template <typename T> static bool ispalindrome(T n, uint8_t base = 10)
{   return n == reverse<T>(n, base);
}

class Kounter
{
private:
    map<uint32_t, uint32_t> _kounter;
public:
    void insert(uint32_t n);
    void dump(ostream &os) const;
    bool hasKey(uint32_t n) const;
    uint32_t max1() const;
    uint32_t max2() const;
    uint32_t min1() const;
    uint32_t min2() const;
    map<uint32_t, uint32_t>::iterator begin() { return _kounter.begin(); }
    map<uint32_t, uint32_t>::iterator end() { return _kounter.end(); }
};

bool Kounter::hasKey(uint32_t n) const
{   for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        if (it->first == n) return true;
    return false;
}

uint32_t Kounter::max1() const
{   uint32_t xmax = 0;
    for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        xmax = std::max(xmax, it->first);
    return xmax;
}

uint32_t Kounter::max2() const
{   uint32_t xmax = 0;
    for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        xmax = std::max(xmax, it->second);
    return xmax;
}

uint32_t Kounter::min1() const
{   uint32_t xmin = 0xffffffff;
    for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        xmin = std::min(xmin, it->first);
    return xmin;
}

uint32_t Kounter::min2() const
{   uint32_t xmin = 0xffffffff;
    for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        xmin = std::min(xmin, it->second);
    return xmin;
}

void Kounter::insert(uint32_t n)
{   if (_kounter.count(n)) _kounter[n]++;
    else _kounter[n] = 1;
}

void Kounter::dump(ostream &os) const
{   for (map<uint32_t, uint32_t>::const_iterator it = _kounter.begin(); it != _kounter.end(); it++)
        os << it->first << ": " << it->second << "\r\n";
}

class Primes
{
private:
    vector<uint64_t> _primes;
    void _add();
public:
    Primes();
    uint32_t get(uint16_t i);
};

class PrimeFactors2 : public Generator<uint64_t>
{
private:
    vector<uint32_t>::iterator _begin;
    vector<uint32_t>::iterator _end;
    uint64_t _n;
public:
    PrimeFactors2(vector<uint32_t>::iterator begin,
        vector<uint32_t>::iterator end, uint64_t n) : _begin(begin), _end(end), _n(n) { }
    bool hasNext() { return _n > 1; }
    uint64_t next();
};

uint64_t PrimeFactors2::next()
{   uint64_t factor = 0;
    for (vector<uint32_t>::const_iterator it = _begin; it != _end; it++)
    {   if (_n % *it == 0)
        {   factor = *it;
            break;
        }
    }
    _n = _n / factor;
    return factor;
}

class PrimeFactors3 : public Generator<uint64_t>
{
private:
    uint32_t *_begin;
    uint32_t *_end;
    uint64_t _n;
public:
    PrimeFactors3(uint32_t *beg, uint32_t *end, uint64_t n) : _begin(beg), _end(end), _n(n) { }
    bool hasNext() { return _n > 1; }
    uint64_t next();
};

uint64_t PrimeFactors3::next()
{   uint64_t factor = 0;
    for (uint32_t *it = _begin; it != _end; it++)
    {   if (_n % *it == 0)
        {   factor = *it;
            break;
        }
    }
    _n = _n / factor;
    return factor;
}

static uint32_t sumProperDivs1(uint32_t n)
{   uint32_t sum = 0;
    for (uint32_t i = 1; i <= n / 2; i++)
        sum += n % i == 0 ? i : 0;
    return sum;
}

static uint32_t sumDivs1(uint32_t n) __attribute__((unused));
static uint32_t sumDivs1(uint32_t n)
{   return sumProperDivs1(n) + n;
}

static uint32_t sumDivs2(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end,
    uint32_t n) __attribute__((unused));
static uint32_t sumDivs2(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end,
    uint32_t n)
{
    PrimeFactors2 gen(begin, end, n);
    uint32_t previous = 0, ret = 1, current = 1;
    while (gen.hasNext())
    {
        uint32_t pf = gen.next();
        if (pf != previous)
        {   ret *= sumDivs1(current);
            current = 1;
        }
        current *= pf;
        previous = pf;
    }
    return ret * sumDivs1(current);
}

static uint32_t sumProperDivs2(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end,
    uint32_t n)
{   return sumDivs2(begin, end, n) - n;
}

static bool isPandigital41(uint32_t n)
{   vector<uint8_t> nset;
    for (uint8_t i = 1; i <= decimals<uint32_t>(n); i++) nset.push_back(i);
    return hasDigitsOnce(n, nset);
}

static uint32_t gcd(uint32_t a, uint32_t b)
{   while (b)
    {   uint32_t c = b;
        b = a % b;
        a = c;
    }
    return a;
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

static uint32_t multiples1(uint32_t limit = 1000)
{   return summation1(3, limit - 1) + summation1(5, limit - 1) - summation1(15, limit - 1);
}

static string problem1()
{   return twostring(multiples1());
}

/*
#2 Each new term in the Fibonacci sequence is generated by adding the previous
two terms. By starting with 1 and 2, the first 10 terms will be:

1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

By considering the terms in the Fibonacci sequence whose values do not
exceed four million, find the sum of the even-valued terms.

Antwoord: 4,613,732
*/

static string problem2(uint32_t xmax = 4000000)
{   uint32_t term1 = 1, term2 = 2, temp = 0, xsum = 2;
    while (true)
    {   temp = term1 + term2;
        if (temp > xmax) break;
        if (temp % 2 == 0) xsum += temp;
        term1 = term2, term2 = temp;
    }
    return twostring(xsum);
}

/*
#3 The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600,851,475,143?

Antwoord: 6,857
*/

static string problem3(uint64_t n = 600851475143ULL)
{   Sieve sieve(9999);
    uint32_t lprimes[8000], i = 0;
    while (sieve.hasNext())
        lprimes[i++] = sieve.next();
    PrimeFactors3 pf3(lprimes, lprimes + (i - 1), n);
    return twostring(max(pf3));
}

/*
#4 A palindromic number reads the same both ways. The largest palindrome
made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Antwoord: 906,609
*/

static string problem4()
{   uint32_t best = 0;
    for (uint32_t a = 0; a < 1000; a++)
    {   for (uint32_t b = 0; b < 1000; b++)
        {   uint32_t c = a * b;
            if (ispalindrome(c) && c > best)
                best = c;
        }
    }
    return twostring(best);
}

/*
#5 2520 is the smallest number that can be divided by
each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly
divisible by all of the numbers from 1 to 20?

Antwoord: 232,792,560
*/

static inline bool isdivisible(uint32_t n, uint32_t lower, uint32_t max)
{   for (uint32_t i = lower; i <= max; i++) if (n % i > 0) return false;
    return true;
}

static string problem5(uint32_t lower = 11, uint32_t max = 20)
{   uint32_t start = 2520, number = start;
    while (isdivisible(number, lower, max) == false) number += start;
    return twostring(number);
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

static string problem6(uint32_t min = 1, uint32_t max = 100)
{   uint32_t sumsquare = 0, squaresum = 0;
    for (uint32_t i = min; i <= max; i++) sumsquare += i * i;
    for (uint32_t i = min; i <= max; i++) squaresum += i;
    squaresum = squaresum * squaresum;
    return twostring(squaresum - sumsquare);
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

static string problem7(uint32_t n = 10001)
{   uint32_t p = 3, sqp = reducer7(p), ret = 0;
    for (uint32_t j = 0; j < n - 1; j++)
    {   for (uint32_t i = 2; i < sqp; i++)
            if (p % i == 0) sqp = reducer7(++p), i = 1;
        ret = p, p += 2, sqp = reducer7(p);
    }
    return twostring(ret);
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

static string problem8(char *s = series1)
{   uint64_t cur = 0, best = 0;
    while (*s)
    {   cur = (cur % 1000000000000ULL) * 10 + (*s++ - '0');
        uint64_t peel = cur, product = 1;
        while (peel) product *= peel % 10, peel = peel / 10;
        if (product > best) best = product;
    }
    return twostring(best);
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

static uint32_t opdracht9(uint32_t search = 1000)
{   for (uint32_t a = 1; a <= search - 2; a++)
    {   for (uint32_t b = 1; b <= search - 2; b++)
        {   uint32_t c = search - a - b;
            if (a * a + b * b == c * c)
                return a * b * c;
        }
    }
    return 0;
}

static string problem9()
{   return twostring(opdracht9());
}

/*
#10 Summation of primes

The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Antwoord: 142,913,828,922
*/

static string problem10(uint32_t limit = 2000000)
{   Sieve sieve(limit);
    return twostring(sum(sieve));
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

static string problem11()
{   uint32_t best = 0;
    for (uint8_t i = 0; i < 20; i++)
    {   for (uint8_t j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i][j+1] * t11[i][j+2] * t11[i][j+3];
            if (prod > best) best = prod;
            prod = t11[j][i] * t11[j+1][i] * t11[j+2][i] * t11[j+3][i];
            if (prod > best) best = prod;
        }
    }
    for (uint8_t i = 0; i < 16; i++)
    {   for (uint8_t j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i+1][j+1] * t11[i+2][j+2] * t11[i+3][j+3];
            if (prod > best) best = prod;
        }
    }
    for (uint8_t i = 3; i < 20; i++)
    {   for (uint8_t j = 0; j < 16; j++)
        {   uint32_t prod = t11[i][j] * t11[i-1][j+1] * t11[i-2][j+2] * t11[i-3][j+3];
            if (prod > best) best = prod;
        }
    }
    return twostring(best);
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

static uint32_t find_triangular_index(uint16_t factor_limit = 500)
{   uint32_t n = 1, lnum = num_divisors(n), rnum = num_divisors(n + 1);
    while (lnum * rnum < factor_limit)
        lnum = rnum, rnum = num_divisors(++n + 1);
    return n;
}

static string problem12(uint16_t divisors = 500)
{   uint32_t i = find_triangular_index(divisors);
    return twostring((i * (i + 1)) >> 1);
}

/*
#13 Large sum

Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.

Antwoord: 5,537,376,230
*/

static string problem13()
{   ifstream ifs;
    ifs.open("euler13.txt");
    uint8_t numbers2[5000];
    uint16_t i = 0;
    while (i < 5000)
    {   int x = ifs.get();
        if (isdigit(x))
            numbers2[i++] = x - '0';
    }
    ifs.close();
    uint8_t totalSum[100];
    uint64_t sum = 0;
    uint8_t end = 0;
    for (uint8_t i = 50; i > 0; i--)
    {   for (uint8_t j = 0; j < 100; j++)
            sum += numbers2[j * 50 + (i - 1)];
        totalSum[end++] = sum % 10;
        sum /= 10;
    }
    for (;sum > 0; sum /= 10)
        totalSum[end++] = sum % 10;
    end--;
    string ret;
    for (uint8_t i = 0; i < 10; i++)
        ret.push_back((char)(totalSum[end--] + 48));
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

static string problem14(uint32_t lower = 1, uint32_t upper = 1000000)
{   uint32_t best_start = 0, best_length = 0;
    for (uint32_t i = lower; i < upper; i++)
    {   uint32_t length = collatz(i);
        if (length > best_length)
            best_start = i, best_length = length;
    }
    return twostring(best_start);
}

/*
#15 Lattice paths

Starting in the top left corner of a 2x2 grid, and only being able to move
to the right and down, there are exactly 6 routes to the bottom right corner.

How many such routes are there through a 20×20 grid?

Antwoord: 137,846,528,820
*/

static string problem15(uint8_t size = 20)
{   uint64_t paths = 1;
    for (uint8_t i = 0; i < size; i++)
        paths = (paths * (2 * size - i)) / (i + 1);
    return twostring(paths);
}

/*
#16 Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Antwoord: 1,366
*/

static string problem16(uint16_t e = 1000)
{   uint8_t largeNum[400];
    memset(largeNum, 0, 400);
    largeNum[0] = 2;
    uint8_t carry = 0;
    uint32_t sum = 0;
    while (--e)
    {   for (uint16_t i = 0; i < sizeof(largeNum); i++)
        {   largeNum[i] *= 2;
            largeNum[i] += carry;
            carry = largeNum[i] / 10;
            largeNum[i] = largeNum[i] % 10;
        }
    }
    for (uint16_t i = 0; i < sizeof(largeNum); i++)
        sum += largeNum[i];
    return twostring(sum);
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
    //cout << s << " ";
    return strlen(s);
}

static string problem17()
{   char arr1[][15] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
        "seventeen", "eighteen", "nineteen",
        "twenty", "thirty", "forty", "fifty", "sixty",
        "seventy", "eighty", "ninety"};
    uint32_t xsum = 0;
    for (uint8_t i = 0; i < 19; i++)
        xsum += len(arr1[i]);
    for (uint8_t i = 19; i < 27; i++)
    {   xsum += len(arr1[i]);
        for (uint8_t j = 0; j < 9; j++)
            xsum += len(arr1[i]) + len(arr1[j]);
    }
    for (uint8_t i = 0; i < 9; i++)
    {   xsum += len(arr1[i]) + len("hundred");
        for (uint8_t j = 0; j < 19; j++)
            xsum += len(arr1[i]) + len("hundred") + len("and") + len(arr1[j]);
        for (uint8_t j = 19; j < 27; j++)
        {   xsum += len(arr1[i]) + len("hundred") + len("and") + len(arr1[j]);
            for (uint8_t k = 0; k < 9; k++)
                xsum += len(arr1[i]) + len("hundredand") + len(arr1[j]) + len(arr1[k]);
        }
    }
    xsum += len("onethousand");
    return twostring(xsum);
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

#if 0
static uint8_t triangle[][4] = {
    {3,0,0,0},
    {7,4,0,0},
    {2,4,6,0},
    {8,5,9,3}};
#else
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
#endif

static string problem18()
{   uint32_t possibilities = myPow<uint64_t>(2, sizeof(triangle[0]) - 1);
    uint32_t best = 0;
    for (uint32_t i = 0; i <= possibilities; i++)
    {   uint32_t index = 0;
        uint32_t sum = triangle[0][0];
        for (uint32_t j = 0; j < sizeof(triangle[0]) - 1; j++)
        {   index = index + (i >> j & 1);
            uint8_t value = triangle[j + 1][index];
            sum += value;
        }
        if (sum > best) best = sum;
    }
    return twostring(best);
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

static const uint8_t TUESDAY = 0, WEDNESDAY = 1, THURSDAY = 2, FRIDAY = 3, SATURDAY = 4,
    SUNDAY = 5, MONDAY = 6;

static string problem19()
{   uint8_t months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint32_t day = 0;
    uint32_t sunday_count = 0;
    for (uint32_t year = 1901; year <= 2000; year++)
    {   bool leap = isLeap(year);
        for (uint8_t m = 0; m < 12; m++)
        {   if (day % 7 == SUNDAY)
                sunday_count++;
            day += months[m];
            if (leap == true && months[m] == 28)
                day++;
        }
    }
    return twostring(sunday_count);
}

/*
#20 Factorial digit sum

n! means n × (n − 1) × ... × 3 × 2 × 1

For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!

Antwoord: 648
*/

static string problem20(uint8_t f = 100)
{   uint16_t buf[200];
    memset(buf, 0, 400);
    buf[0] = f;
    for (uint8_t i = f - 1; i > 0; i--)
    {   uint16_t carry = 0;
        for (uint8_t j = 0; j < 200; j++)
        {   buf[j] *= i;
            buf[j] += carry;
            carry = buf[j] / 10;
            buf[j] = buf[j] % 10;
        }
    }
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 200; i++) sum += buf[i];
    return twostring(sum);
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

static string problem21(uint32_t low = 1, uint32_t high = 10000)
{   uint32_t l[high - low];
    memset(l, 0, (high - low) * 4);
    for (uint32_t i = low; i <= high; i++)
        l[i - low] = sumProperDivs1(i);
    uint32_t sum = 0;
    for (uint32_t i = 0; i <= (high - low); i++)
    {   uint32_t ind = l[i];
        if (i + low < ind && low <= ind && ind <= high && l[ind - low] == i + low)
            sum += (i + low) + ind;
    }
    return twostring(sum);
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

static const uint8_t letterwaarde(uint8_t c)
{   return c > 64 ? c - 64 : c;
}

static string problem22()
{   ifstream ifs;
    ifs.open("euler22.txt");
    vector<string> vec;
    string tmp;
    while (getline(ifs, tmp))
        if (tmp.size() > 0)
            vec.push_back(tmp);
    ifs.close();
    sort(vec.begin(), vec.end());
    uint32_t total = 0;
    for (uint32_t i = 0; i < vec.size(); i++)
    {   uint32_t score = 0;
        string tmp = vec.at(i);
        for (uint32_t j = 0; j < tmp.size(); j++)
            score += letterwaarde(tmp.at(j));
        score = score * (i + 1);
        total += score;
    }
    return twostring(total);
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

static bool find23(vector<uint16_t> &d, uint16_t n)
{
    return binSearch(d.begin(), d.end(), n);
}

static string problem23()
{   uint16_t xmax = 28123;
    vector<uint32_t> lprimes;
    Sieve sieve(99999);
    while (sieve.hasNext())
        lprimes.push_back(sieve.next());
    vector<uint16_t> abundants;
    for (uint16_t i = 1; i <= xmax; i++)
        if (sumProperDivs2(lprimes.begin(), lprimes.end(), i) > i)
            abundants.push_back(i);
    uint32_t xsum = 1;
    for (uint16_t i = 2; i <= xmax; i++)
    {   bool boo = true;
        for (vector<uint16_t>::const_iterator it = abundants.begin(); it != abundants.end(); it++)
        {   if (*it < i)
            {   if (find23(abundants, i - *it))
                {   boo = false;
                    break;
                }
            } else break;
        }
        if (boo == true) xsum += i;
    }
    return twostring(xsum);
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

static string problem24()
{   uint8_t a[] = {0,1,2,3,4,5,6,7,8,9}, r = 0, j;
    char ret[11];
    uint32_t perm = 999999;
    for (j = 0; j < 10; j++)
    {   uint8_t i = perm / fac32(9 - j);
        perm = perm % fac32(9 - j);
        ret[r++] = a[i] + '0';
        xmemcpy(a + i, a + i + 1, 9 - i);
    }
    ret[10] = 0;
    return string(ret);
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

static string problem25()
{   uint8_t i = 0;
    uint16_t cnt = 2;
    LongNumber25 fib[3];
    fib[0] = 1;
    fib[1] = 0;
    fib[2] = 1;
    while (fib[i].digits() < 1000)
    {   i = (i + 1) % 3;
        cnt++;
        LongNumber25 tmp = fib[(i + 1) % 3];
        tmp += fib[(i + 2) % 3];
        fib[i] = tmp;
    }
    return twostring(cnt);
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
static uint32_t cycleLength(uint32_t n)
{   uint32_t a = 1, t = 0;
    while (t < n)
    {   a = a * 10 % n, t++;
        if (a == 0) return 0;
        if (a == 1) return t;
    }
    return 0;
}

static string problem26()
{   uint32_t best_n = 0, best_length = 0;
    for (uint32_t i = 999; i > 1; i--)
    {   uint32_t clength = cycleLength(i);
        if (clength > best_length)
        {   best_n = i, best_length = clength;
            if (best_length == i - 1) break;
        }
    }
    return twostring<uint32_t>(best_n);
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

static string problem27()
{   int32_t best_a = 0, best_b = 0, best_n = 0;
    for (int32_t a = -999; a < 1000; a++)
    {   for (int32_t b = -1000; b <= 1000; b++)
        {   int32_t n = 0;
            while (isPrime(abs(n * n + a * n + b))) n++;
            if (n > best_n) best_a = a, best_b = b, best_n = n;
        }
    }
    return to_string32s(best_a * best_b);
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

static string problem28(uint32_t root = 1001)
{   uint32_t xsum = 1;    
    for (uint32_t step = 2, foo = 1; foo < root * root; step += 2)
        for (uint8_t i = 0; i < 4; i++) foo += step, xsum += foo;
    return twostring(xsum);
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
{   for (uint8_t a = 2; a <= 10; a++)
        for (uint32_t e = 1, b = 0; (b = myPow<uint32_t>(a, e)) <= 100; e++)
            if (b == (n & 0xffff0000) >> 16) return a << 16 | e;
    return n;
}

static uint32_t eqpow(uint32_t n)
{   uint32_t root = myRoot(n);
    return (root & 0xffff0000) | (n & 0xffff) * (root & 0xffff);
}

static string problem29()
{   uint32_t buf[99*99];
    for (uint16_t a = 2; a <= 100; a++)
        for (uint16_t b = 2; b <= 100; b++)
            buf[(a-2)*99+(b-2)] = eqpow(a << 16 | b);
    bubbleSort(buf, buf + 99*99);
    uint32_t *foo = unique(buf, buf + 99*99);
    return twostring(foo - buf);
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
    while (tmp > 0) xsum += myPow<uint32_t>(tmp % 10, p), tmp = tmp / 10;
    return xsum == n;
}

static string problem30(uint8_t p = 5)
{   uint32_t xsum = 0;
    for (uint32_t i = 2; i < 1000000; i++) xsum += test30(i, p) ? i : 0;
    return twostring<uint32_t>(xsum);
}

/*
#31 Coin sums

In England the currency is made up of pound, P, and pence,
p, and there are eight coins in general circulation:

    1p, 2p, 5p, 10p, 20p, 50p, P1 (100p) and P2 (200p).

It is possible to make £2 in the following way:

    1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p

How many different ways can £2 be made using any number of coins?

Antwoord: 73,682
*/

static string problem31()
{   uint8_t target = 200, coins[] = {1,2,5,10,20,50,100,200};
    uint32_t ways[target + 1];
    memset(ways, 0, (target + 1) * 4);
    ways[0] = 1;
    for (uint8_t i = 0; i < sizeof(coins); i++)
        for (uint8_t j = coins[i]; j <= target; j++)
            ways[j] += ways[j - coins[i]];
    return twostring<uint32_t>(ways[target]);
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

static void panProducts32(vector<uint32_t> &st)
{   for (uint32_t i = 2; i < 60; i++)
    {   uint32_t start = i < 10 ? 1234 : 123;
        for (uint32_t j = start; j < 10000/i; j++)
        {   vector<uint8_t> nset;
            for (uint8_t n = 1; n <= 9; n++) nset.push_back(n);
            if (hasDigitsOnce<uint32_t>(i, nset) == false) continue;
            if (hasDigitsOnce<uint32_t>(j, nset) == false) continue;
            if (hasDigitsOnce<uint32_t>(i * j, nset)  == false) continue;
            st.push_back(i*j);
        }
    }
}

static string problem32()
{   vector<uint32_t> st;
    panProducts32(st);
    sort(st.begin(), st.end());
    vector<uint32_t>::iterator foo = unique(st.begin(), st.end());
    uint32_t xsum = 0;
    for (vector<uint32_t>::const_iterator it = st.begin(); it != foo; it++) xsum += *it;
    return twostring<uint32_t>(xsum);
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

static string problem33()
{   uint32_t dp = 1, np = 1;
    for (uint32_t c = 1; c < 10; c++)
    {   for (uint32_t d = 1; d < c; d++)
        {   for (uint32_t n = 1; n < d; n++)
                if ((n * 10 + c) * d == (c * 10 + d) * n)
                    np *= n, dp *= d;
        }
    }
    return twostring<uint32_t>(dp / gcd(np, dp));
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

static string problem34()
{   uint32_t totalSum = 0;
    for (uint32_t k = 10; k < factorials34[9] * 7; k++)
        if (facsumdig34(k) == k) totalSum += k;
    return twostring(totalSum);
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
{   uint32_t length = decimals(n), digit = n % 10;
    return n / 10 + digit * myPow<uint32_t>(10, length - 1);
}

static bool iscircular(uint32_t n, uint32_t *begin, uint32_t *end)
{   uint8_t decs = decimals(n);
    for (uint8_t i = 0; i < decs; i++)
    {   n = rotate(n);
        if (binSearch(begin, end, n) == false) return false;
    }
    return true;
}

static string problem35()
{   uint32_t *primes = new uint32_t[80000], end = 0, ncount = 0;
    Sieve sieve(999999);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    for (uint32_t *it = primes; it != primes + (end - 1); it++)
        if (iscircular(*it, primes, primes + (end - 1))) ncount++;
    delete[] primes;
    return twostring(ncount);
}

/*
#36: Double-base palindromes

The decimal number, 585 = 1001001001_2 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base, may not include leading zeros.)

Antwoord: 872,187
*/

static string problem36(uint32_t min = 1, uint32_t limit = 1000000)
{   uint32_t xsum = 0;
    for (uint32_t i = min; i < limit; i++)
        if (ispalindrome(i, 10) && ispalindrome(i, 2)) xsum += i;
    return twostring(xsum);
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

template <typename T>
static bool isrighttruncatable(uint32_t prime, T begin, T end)
{   while (prime > 10)
    {   prime = prime / 10;
        if (binSearch(begin, end, prime) == false) return false;
    }
    return true;
}

static uint32_t truncate_left(uint32_t n)
{   uint8_t exp = decimals(n) - 1;
    return n % myPow<uint32_t>(10, exp);
}

template <typename T>
static bool islefttruncatable(uint32_t prime, T begin, T end)
{   uint8_t length = decimals(prime);
    for (uint8_t i = 0; i < length; i++)
    {   if (binSearch(begin, end, prime) == false) return false;
        prime = truncate_left(prime);
    }
    return true;
}

static string problem37()
{   uint32_t *primes = new uint32_t[80000], end = 0, xsum = 0;
    Sieve sieve(999999);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    end--;
    for (uint32_t *it = primes; it != primes + end; it++)
    {   if (*it == 2 || *it == 3 || *it == 5 || *it == 7)
            continue;
        if (islefttruncatable(*it, primes, primes + end) &&
                isrighttruncatable(*it, primes, primes + end))
        {
            xsum += *it;
        }
    }
    delete[] primes;
    return twostring(xsum);
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

static uint32_t opdracht38()
{   for (uint32_t i = 9387; i > 9234; i--)
    {   uint32_t result = 2 * i + i * 100000;
        if (isPandigital41(result)) return result;
    }
    return 0;
}

static string problem38()
{   return twostring(opdracht38());
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

static string problem39()
{   uint32_t best_p = 0, best_solutions = 0;
    for (uint32_t p = 100; p <= 1000; p += 2)
    {   uint32_t solutions = 0;
        for (uint32_t a = 2; a < p / 3; a++)
            solutions += (p * (p - 2 * a) % (2 * (p - a)) == 0) ? 1 : 0;
        if (solutions > best_solutions) best_solutions = solutions, best_p = p;
    }
    return twostring(best_p);
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
{   uint32_t offset = 0, decimals = 1, setLow = 1, setLength = 9;
    for (uint32_t limit = 9; i >= limit; limit += setLength * decimals)
        offset = limit, decimals++, setLow *= 10, setLength *= 10;
    uint32_t n = (i - offset) / decimals + setLow, ind = (i - offset) % decimals;
    for (uint32_t x = 0; x < decimals - (ind + 1); x++) n /= 10;
    return n % 10;
}

static string problem40()
{   uint32_t product = 1;
    for (uint32_t i = 1; i <= 1000000; i *= 10) product *= getDigit40(i - 1);
    return twostring<uint32_t>(product);
}

/*
#41: Pandigital prime

We shall say that an n-digit number is pandigital if it makes use of all
the digits 1 to n exactly once. For example, 2143 is a 4-digit pandigital
and is also prime.

What is the largest n-digit pandigital prime that exists?

Antwoord: 7,652,413
*/

static string problem41()
{
    uint32_t *primes = new uint32_t[600000], end = 0, best = 0;
    Sieve sieve(7654321);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    end--;
    for (uint32_t *it = primes; it != primes + end; it++)
        if (isPandigital41(*it) && *it > best) best = *it;
    delete[] primes;
    return twostring(best);
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

static uint32_t wordcount42(const char *w)
{   uint32_t count = 0;
    while (*w) count += *w++ - 64;
    return count;
}

static string problem42()
{   vector<string> words;
    ifstream ifs;
    ifs.open("euler42.txt");
    string tmp;
    while (getline(ifs, tmp))
        if (tmp.size() > 0)
            words.push_back(tmp);
    ifs.close();
    vector<uint32_t> triangles;
    for (uint32_t i = 0; i < 20; i++) triangles.push_back(triangle32(i));
    uint32_t ret = 0;
    for (vector<string>::iterator it = words.begin(); it != words.end(); it++)
        if (linSearch(triangles.begin(), triangles.end(), wordcount42(it->c_str())))
            ret++;
    return twostring<uint32_t>(ret);
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

class Permutations
{
private:
    vector<uint8_t> _pool;
    uint32_t _size;
    uint32_t _i;
    uint8_t *_c;
public:
    Permutations(uint8_t max);
    ~Permutations() { delete[] _c; }
    bool hasNext() { return _i < _size; }
    uint64_t next();
};

Permutations::Permutations(uint8_t max)
{   for (uint8_t i = 0; i <= max; i++)
        _pool.push_back(i);
    _size = max + 1;
    _i = 0;
    _c = new uint8_t[_size];
    memset(_c, 0, _size);
}

static uint64_t concat43(vector<uint8_t> &v)
{   uint64_t ret = 0;
    for (uint32_t i = 0; i < v.size(); i++)
        ret += v.at(v.size() - 1 - i) * myPow<uint64_t>(10, i);
    return ret;
}

uint64_t Permutations::next()
{   uint32_t tmp = 0;
    while (hasNext())
        if (_c[_i] < _i)
        {   if (_i % 2 == 0) tmp = _pool[0], _pool[0] = _pool[_i], _pool[_i] = tmp;
            else tmp = _pool[_c[_i]], _pool[_c[_i]] = _pool[_i], _pool[_i] = tmp;
            _c[_i]++, _i = 0;
            return concat43(_pool);
        } else _c[_i++] = 0;
    return 0;   // moet eigenlijk origineel teruggeven
}

static bool test43(uint64_t n)
{   uint8_t divs[] = {17,13,11,7,5,3,2};
    for (uint8_t i = 0; i < 7; i++)
        if ((n / myPow<uint32_t>(10, i) % 1000) % divs[i] != 0)
            return false;
    return true;
}

static string problem43()
{   Permutations perms(9);
    //vector<uint64_t> ps;
    uint64_t xsum = 0;
    while (perms.hasNext())
    {   uint64_t p = perms.next();
        xsum += test43(p) ? p : 0;
    }
    return twostring(xsum);
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
{   uint32_t lpgs[9998];
    for (uint32_t i = 1; i <= 9998; i++) lpgs[i - 1] = pentagon32(i);
    for (uint32_t i = 0; i < 9998; i++)
        for (uint32_t j = i; j < 9998; j++)
            if (binSearch(lpgs + j, lpgs + 9998, lpgs[i] + lpgs[j]) &&
                binSearch(lpgs, lpgs + 9998, lpgs[j] - lpgs[i]))
                return lpgs[j] - lpgs[i];
    return 0;
}

static string problem44()
{   return twostring(opdracht44());
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

// T65,535 = H32768 = 2,147,450,880

static uint32_t opdracht45()
{   uint32_t vp[31834], vh[45856];
    for (uint32_t i = 166; i < 32000; i++) vp[i - 166] = pentagon32(i);
    for (uint32_t i = 144; i < 46000; i++) vh[i - 144] = hexagon32(i);
    for (uint32_t *it = vh; it != vh + 45856; it++)
        if (binSearch(vp, vp + 31834, *it)) return *it;
    return 0;
}

static string problem45()
{   return twostring(opdracht45());
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

static uint64_t pair46(vector<uint32_t> &primes, vector<uint32_t> &squares, uint32_t n)
{   for (vector<uint32_t>::iterator it = primes.begin(); it != primes.end(); it++)
    {   if (*it > n) break;
        if (binSearch(squares.begin(), squares.end(), n - *it))
            return (uint64_t)*it << 32 | (n - *it);
    }
    return 0;
}

static uint32_t opdracht46()
{   vector<uint32_t> primes, squares;
    Sieve sieve(999999);
    while (sieve.hasNext()) primes.push_back(sieve.next());
    for (uint32_t i = 0; i < 100; i++) squares.push_back(2*i*i);
    for (uint32_t i = 3; i < 987654321; i += 2)
    {   if (binSearch(primes.begin(), primes.end(), i)) continue;
        uint64_t pr = pair46(primes, squares, i);
        if (pr == 0) return i;
    }
    return 0;
}

static string problem46()
{   return twostring(opdracht46());
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

static uint32_t opdracht47(uint32_t L = 300000, uint32_t nf = 4, uint32_t ns = 4)
{   uint32_t f[L], c = 0;
    memset(f, 0, 4*L);
    for (uint32_t n = 2; n < L - ns; n++)
    {   if (f[n] == nf) { if (++c == ns) return n - ns + 1; }
        else { c = 0; if (f[n] == 0) for (uint32_t i = n; i < L; i += n) f[i]++; }
    }
    return 0;
}

static string problem47()
{   return twostring<uint32_t>(opdracht47());
}

/*
#48: Self powers
*/

static string problem48()
{   uint64_t result = 0, modulo = 10000000000ULL;
    for (uint16_t i = 1; i <= 1000; i++)
    {   uint64_t temp = i;
        for (uint16_t j = 1; j < i; j++) temp *= i, temp %= modulo;
        result += temp, result %= modulo;
    }
    return twostring<uint64_t>(result);
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

static uint64_t check(uint32_t *begin, uint32_t *end)
{
    for (uint32_t *it = begin; it != end; it++)
    {   if (binSearch(begin, end, *it + 3330) &&
            binSearch(begin, end, *it + 6660) &&
            sameDigs<uint32_t>(*it, *it + 3330) && sameDigs<uint32_t>(*it, *it + 6660))
        {   return (*it + 6660) + (*it + 3330) * 10000 + (uint64_t)*it * 100000000;
        }
    }
    return 0;
}

static string problem49()
{
    uint32_t primes[8000], begin = 0, end = 0;
    Sieve sieve(9999);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    while (primes[begin++] < 1487);
    return twostring<uint64_t>(check(primes + begin, primes + (end - 1)));
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

static string problem50(uint32_t limit = 1000000)
{   uint32_t *primes = new uint32_t[80000], end = 0;
    Sieve sv(999999);
    while (sv.hasNext()) primes[end++] = sv.next();
    end--;
    uint32_t best_prime = 0, best_sum = 0;
    for (uint32_t i = 0; i < end; i++)
    {   for (uint32_t j = i + best_sum; j < end; j++)
        {   uint32_t xsum = 0;
            for (uint32_t k = i; k <= j; k++) xsum += primes[k];
            if (xsum >= limit) break;
            uint32_t sublen = (j + 1) - i;
            if (binSearch(primes, primes + end, xsum) && sublen > best_sum)
                best_sum = sublen, best_prime = xsum;
        }
    }
    return twostring(best_prime);
}

/*
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
*/

static void binarize51(vector<uint32_t> &out, uint32_t n)
{   uint32_t div = 2, dec = 1;
    while (n)
    {   out.push_back(n % div ? dec : 0);
        n -= n % div;
        div *= 2;
        dec *= 10;
    }
}

static void
family51(vector<uint32_t> &out, vector<uint32_t> &primes, uint32_t n, uint32_t mask)
{   uint32_t xlen = decimals<uint32_t>(n);
    vector<uint32_t> bmask;
    binarize51(bmask, mask);
    for (uint32_t i = 0; i < bmask.size(); i++)
        n -= bmask.at(i) * digit<uint32_t>(n, i);
    for (uint32_t i = 0; i < 10; i++)
    {   uint32_t tmp = n;
        for (vector<uint32_t>::iterator it = bmask.begin(); it != bmask.end(); it++)
            tmp += *it * i;
        if (decimals<uint32_t>(tmp) == xlen && binSearch(primes.begin(), primes.end(), tmp))
            out.push_back(tmp);
    }
}

static uint32_t opdracht51()
{   vector<bool> v(1000000, true);
    v[0] = v[1] = false;
    for (uint32_t p = 2; p * p < v.size(); p++)
        if (v[p]) for (uint32_t i = p * 2; i < v.size(); i += p) v[i] = false;
    vector<uint32_t> primes;
    for (uint32_t i = 0; i < v.size(); i++) if (v[i]) primes.push_back(i);
    for (vector<uint32_t>::iterator it = primes.begin(); it != primes.end(); it++)
    {   for (uint32_t mask = 1; mask < myPow<uint32_t>(2, decimals<uint32_t>(*it)); mask++)
        {   vector<uint32_t> fam;
            family51(fam, primes, *it, mask);
            if (fam.size() == 8) return fam.at(0);
        }
    }
    return 0;
}

static string problem51()
{   return twostring<uint32_t>(opdracht51());
}

/*
#52: Pandigital multiples

It can be seen that the number, 125874, and its double, 251748,
contain exactly the same digits, but in a different order.

Find the smallest positive integer, x, such that
2x, 3x, 4x, 5x, and 6x, contain the same digits.

Antwoord: 142,857
*/

static bool test52(uint32_t n)
{   vector<uint8_t> nset;
    for (uint32_t x = n; x; x = x / 10) nset.push_back(x % 10);
    for (uint32_t m = 2; m <= 6; m++)
    {   vector<uint8_t> nset2;
        for (vector<uint8_t>::iterator it = nset.begin(); it != nset.end(); it++)
            nset2.push_back(*it);
        if (hasDigitsOnce<uint32_t>(n * m, nset2) == false) return false;
    }
    return true;
}

static uint32_t opdracht52()
{   for (uint32_t n = 2; n < 200000; n++) if (test52(n)) return n;
    return 0;
}

static string problem52()
{   return twostring<uint32_t>(opdracht52());
}

/*
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
for 1 <= n <= 100, are greater than one-million?

Antwoord: 4,075
*/

static string problem53()
{   static const uint8_t nlimit = 101;
    static const uint32_t limit = 1000000;
    uint32_t tree[nlimit][nlimit];
    for (uint8_t i = 0; i < nlimit; i++)
        for (uint8_t j = 0; j < nlimit; j++)
            tree[i][j] = 0;
    tree[0][0] = 1;
    for (uint8_t i = 0; i < nlimit - 1; i++)
    {   tree[i + 1][0] = 1;
        for (uint8_t j = 0; j <= i; j++)
        {   tree[i + 1][j + 1] = tree[i][j] + tree[i][j + 1];
            if (tree[i + 1][j + 1] > 1000001)
                tree[i + 1][j + 1] = 1000001;
        }
    }
    uint32_t ncount = 0;
    for (uint8_t i = 0; i < nlimit; i++)
        for (uint8_t j = 0; j < nlimit; j++)
            if (tree[i][j] > limit) ncount++;
    return twostring<uint32_t>(ncount);
}

/*
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
*/

static uint32_t parseHand(uint16_t *cards)
{   char value[] = "23456789TJQKA";
    char suit[] = "CDHS";
    uint32_t ret = 0;
    for (uint8_t i = 0; i < 5; i++)
    {   char x = cards[i] & 0xff;
        for (uint8_t j = 0; j <= 12; j++)
            if (x == value[j])
                ret |= (j + 2) << (i * 6);
        x = (cards[i] & 0xff00) >> 8;
        for (uint8_t j = 0; j <= 4; j++)
            if (x == suit[j])
                ret |= j << (i * 6 + 4);
    }
    return ret;
}

static uint64_t parse(string s)
{   uint16_t cards[10];
    for (uint8_t i = 0, j = 0; i < s.size(); i += 3)
        cards[j++] = s.at(i) | s.at(i + 1) << 8;
    uint32_t p1 = parseHand(cards), p2 = parseHand(cards + 5);
    return p1 | (uint64_t)p2 << 32;
}

static uint32_t straight(Kounter &cnt)
{   uint32_t xmax = cnt.max1();
    for (uint8_t i = 0; i < 5; i++)
        if (cnt.hasKey(xmax - i) == false) return 0;
    return xmax;
}

static bool flush(Kounter &cnt)
{   return cnt.max2() == 5;
}

static uint32_t fullHouse(Kounter &cnt)
{   return cnt.max2() == 3 && cnt.min2() == 2;
}

static uint32_t fourkind(Kounter &cnt)
{   return cnt.max2() == 4;
}

static uint32_t threekind(Kounter &cnt)
{   return cnt.max2() == 3 && cnt.min2() == 1;
}

static uint32_t twopair(Kounter &cnt)
{   uint32_t best = 0;
    uint32_t pairCount = 0;
    for (map<uint32_t, uint32_t>::iterator it = cnt.begin(); it != cnt.end(); it++)
    {   if (it->second == 2)
        {   pairCount++;
            best = max(best, it->first);
        }
    }
    return pairCount == 2 ? best : 0;
}

static uint32_t onepair(Kounter &cnt)
{   uint32_t bestKicker = 0;
    uint32_t pairValue = 0;
    uint32_t pairCount = 0;
    for (map<uint32_t, uint32_t>::iterator it = cnt.begin(); it != cnt.end(); it++)
    {   if (it->second == 2)
        {   pairCount++;
            pairValue = it->first;
        }
        else if (it->second == 1)
        {   bestKicker = max(bestKicker, it->first);
        }
    }
    return pairCount == 1 ? pairValue * 15 + bestKicker : 0;
}

static uint32_t score(uint32_t hand)
{   Kounter values, suits;
    for (uint8_t i = 0; i < 5; i++)
    {   values.insert(hand >> i * 6 & 0xf);
        suits.insert(hand >> (i * 6 + 4) & 3);
    }
    if (fourkind(values)) return 5004;
    if (fullHouse(values)) return 5003;
    if (flush(suits)) return 5002;
    if (straight(values)) return 5001;
    if (threekind(values)) return 5000;
    if (twopair(values)) return twopair(values) + 300;
    if (onepair(values)) return onepair(values);
    return values.max1();   // high card
}

static void translate(uint64_t hand, ostream &os) __attribute__((unused));
static void translate(uint64_t hand, ostream &os)
{   char value[] = "23456789TJQKA";
    char suit[] = "CDHS";
    uint32_t player1 = hand & 0xffffffff, player2 = (hand & 0xffffffff00000000ULL) >> 32;
    for (uint8_t i = 0; i < 5; i++)
    {   os.put(value[(player1 >> i * 6 & 0xf) - 2]);
        os.put(suit[player1 >> (i * 6 + 4) & 3]);
        os.put(' ');
    }
    for (uint8_t i = 0; i < 5; i++)
    {   os.put(value[(player2 >> i * 6 & 0xf) - 2]);
        os.put(suit[player2 >> (i * 6 + 4) & 3]);
        os.put(' ');
    }
}

static string problem54()
{   vector<uint64_t> twoHands;
    ifstream ifs;
    ifs.open("euler54.txt");
    string tmp;
    while (getline(ifs, tmp))
        if (tmp.size() > 0)
            twoHands.push_back(parse(tmp));
    ifs.close();
    uint32_t ret = 0;
    for (vector<uint64_t>::iterator it = twoHands.begin(); it != twoHands.end(); it++)
        if (score(*it & 0xffffffff) > score((*it & 0xffffffff00000000ULL) >> 32))
            ret++;
    return twostring<uint32_t>(ret);
}

/*
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
*/

static uint8_t isLychrel(LongNumber25 n, uint8_t it = 50)
{   while (it--)
    {   n += n.reverse();
        if (n.ispalindrome()) return 0;
    }
    return 1;
}

static string problem55()
{   uint16_t ret = 0;
    for (uint16_t i = 0; i < 10000; i++)
        ret += isLychrel(LongNumber25(i));
    return twostring<uint16_t>(ret);
}

/*
#56: Powerful digit sum

A googol (10^100) is a massive number: one followed by one-hundred zeros;
100^100 is almost unimaginably large: one followed by two-hundred zeros.
Despite their size, the sum of the digits in each number is only 1.

Considering natural numbers of the form, ab,
where a, b < 100, what is the maximum digital sum?

Antwoord: 972
*/

/*
http://euler.stephan-brumme.com/56/
*/

struct BigNum : public std::vector<unsigned int>
{   static const uint32_t MaxDigit = 10;
    BigNum(uint64_t x = 0)
    {   do
        {   push_back(x % MaxDigit);
            x /= MaxDigit;
        } while (x > 0);
    }
    BigNum operator+(const BigNum& other) const
    {   BigNum result = *this;
        if (result.size() < other.size())
            result.resize(other.size(), 0);
        uint32_t carry = 0;
        for (size_t i = 0; i < result.size(); i++)
        {   carry += result[i];
            if (i < other.size()) carry += other[i];
            else if (carry == 0) return result;
            if (carry < MaxDigit) result[i] = carry, carry = 0;
            else result[i] = carry - MaxDigit, carry = 1;
        }
        if (carry > 0) result.push_back(carry);
        return result;
    }
    BigNum operator*(uint32_t factor) const
    {
        uint64_t carry = 0;
        BigNum result = *this;

        for (auto &i : result)
        {
            carry += i * (uint64_t)factor;
            i      = carry % MaxDigit;
            carry /= MaxDigit;
        }
        while (carry > 0)
        {
            result.push_back(carry % MaxDigit);
            carry /= MaxDigit;
        }
        return result;
    }
};

static string problem56()
{   uint32_t maximum = 100, maxSum = 1;
    for (unsigned int base = 1; base <= maximum; base++)
    {   BigNum power = 1;
        for (uint32_t exponent = 1; exponent <= maximum; exponent++)
        {
            unsigned int sum = 0;
            for (auto digit : power) sum += digit;
            if (maxSum < sum) maxSum = sum;
            power = power * base;
        }
    }
    return twostring<uint32_t>(maxSum);
}

/*
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
*/

/*
http://euler.stephan-brumme.com/57/
*/

static string problem57()
{   uint32_t iterations = 1000;
    BigNum a = 1, b = 1;
    uint32_t count = 0;
    for (uint32_t i = 0; i <= iterations; i++)
    {   if (a.size() > b.size()) count++;
        BigNum twoB  = b + b;
        BigNum nextA = a + twoB;
        BigNum nextB = b + a;
        a = std::move(nextA);
        b = std::move(nextB);
    }
    return twostring<uint32_t>(count);
}

/*
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
*/

/*
https://www.mathblog.dk/project-euler-58-primes-diagonals-spiral/
*/

static string problem58()
{   uint32_t sl = 2, cnt = 3, c = 9;
    while ((double)cnt / (2*sl+1) >= 0.10)
    {   sl += 2;
        for (uint8_t i = 0; i < 3; i++)
        {   c += sl;
            if (isPrime(c)) cnt++;
        }
        c += sl;
    }
    return twostring<uint32_t>(sl + 1);
}

/*
#59: XOR decryption

Each character on a computer is assigned a unique code and the preferred
standard is ASCII (American Standard Code for Information Interchange). For
example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.

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
message. The balance for this method is using a sufficiently long
password key for security, but short enough to be memorable.

Your task has been made easy, as the encryption key consists of three lower
case characters. Using cipher.txt (right click and 'Save Link/Target As...'),
a file containing the encrypted ASCII codes, and the knowledge that the
plain text must contain common English words, decrypt the message and find
the sum of the ASCII values in the original text.

Antwoord: 107,359
*/

static string problem59()
{
    vector<uint8_t> msg;
    ifstream ifs;
    ifs.open("euler59.txt");
    char c;
    uint8_t n = 0;
    while (ifs.get(c))
    {
        if (isdigit(c) == false)
        {
            msg.push_back(n);
            n = 0;
            continue;
        }
        n = n * 10 + (c - 48);

    }
    ifs.close();
#if 0
    for (vector<uint8_t>::iterator it = msg.begin(); it != msg.end(); it++)
        cout << (uint16_t)*it << "\r\n";
#endif
    for (uint8_t i = 97; i < 123; i++)
    {
        for (uint8_t j = 97; j < 123; j++)
        {
            for (uint8_t k = 97; k < 123; k++)
            {
                
            }
        }
    }
    return twostring<uint32_t>(0);
}

/*
#60: Prime pair sets

The primes 3, 7, 109, and 673, are quite remarkable. By taking any two
primes and concatenating them in any order the result will always be
prime. For example, taking 7 and 109, both 7109 and 1097 are prime. The
sum of these four primes, 792, represents the lowest sum for a set of
four primes with this property.

Find the lowest sum for a set of five primes for which any two primes
concatenate to produce another prime.

Antwoord: 26,033
*/

/*
13 + 5,197 + 5,701 + 6,733 + 8,389 = 26,033
*/

static string problem60()
{
    return twostring<uint32_t>(0);
}

static string problem61()
{
    return twostring<uint32_t>(0);
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

static string problem62()
{
    uint64_t lst[9000];
    for (uint64_t n = 0; n < 9000; n++)
        lst[n] = n * n * n;
    for (uint32_t i = 0; i < 9000; i++)
    {
        uint32_t cnt = 0;
        uint8_t ln = decimals<uint64_t>(lst[i]);
        for (uint32_t b = i; b < 9000; b++)
        {
            if (decimals(lst[b]) > ln) break;
            if (sameDigs<uint64_t>(lst[i], lst[b])) cnt++;
        }
        if (cnt == 5)
            return twostring<uint64_t>(lst[i]);
    }
    return twostring<uint64_t>(0);
}

/*
Einde opdrachten
*/

static string run2(uint32_t p)
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
    case 51: return problem51();
    case 52: return problem52();
    case 53: return problem53();
    case 54: return problem54();
    case 55: return problem55();
    case 56: return problem56();
    case 57: return problem57();
    case 58: return problem58();
    case 59: return problem59();
    case 60: return problem60();
    case 61: return problem61();
    case 62: return problem62();
    }
    return 0;
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


#ifdef MULTITHREAD
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args)-> future<typename result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    vector<thread> workers;
    queue<function<void()> > tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
};

inline ThreadPool::ThreadPool(size_t threads) : stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}

template<class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args&&... args)->future<typename result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    future<return_type> res = task->get_future();
    {
        unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
#endif

volatile bool simple_locked = false;

static void runjob2(uint32_t n)
{
    time_t begin = time(0);
    string answer = run2(n);

    if (answer.compare(answers2[n - 1]) != 0)
        throw "error";

    time_t end = time(0);
    cout << "#" << (uint16_t)n << ": " << answer << " " << end - begin << "s\r\n";
}

#ifdef MULTITHREAD
static void multithread(uint8_t max)
{
    ThreadPool pool(4);
    std::vector<std::future<void> > results;

    for (uint8_t i = 1; i <= max; ++i)
        results.emplace_back(pool.enqueue([i] { runjob2(i); }));
}
#else
static void singlethread(uint8_t max)
{
    for (uint8_t i = 1; i <= max; i++)
        runjob2(i);
}
#endif

int main()
{
    time_t begin = time(0);
    //strcpy(answers2[43-1], "0");
#ifdef MULTITHREAD
    multithread(59);
#else
    singlethread(62);
#endif
    time_t end = time(0);
    cout << "Total: " << end - begin << "s\r\n";
    return 0;
}


