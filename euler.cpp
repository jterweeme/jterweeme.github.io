//#define MULTITHREAD

#include <iostream>
#include <fstream>
#include <ctime>
#ifdef MULTITHREAD
#include <vector>
#include <queue>
#include <functional>
#include <future>
#endif
using namespace std;

size_t xstrlen(const char *s)
{   size_t i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

int xstrcmp(const char* s1, const char* s2)
{   while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const uint8_t*)s1-*(const uint8_t*)s2;
}

void xmemcpy(const void *dest, const void *src, size_t n)
{   char *csrc = (char *)src;
    char *cdest = (char *)dest;
    size_t i;
    for (i = 0; i<n; i++) cdest[i] = csrc[i];
}

void *xmemset(void *s, int c, size_t n)
{   uint8_t *p = (uint8_t *)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}

template <typename T> void xswap(T &a, T &b)
{
    T c = a; a=b; b=c;
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

template <typename T> static T factorial(T n)
{   T product = 1;
    while (n > 1) product *= n--;
    return product;
}

template <typename T> static T digit(T n, T i)
{   return n / myPow<T>(10, i) % 10;
}

#if 0
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
#endif

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

static uint64_t powmod(uint64_t base, uint64_t exponent, uint64_t modulo)
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

#if 0
static uint64_t last(Generator<uint32_t> &s)
{   uint64_t ret = 0;
    while (s.hasNext()) ret = s.next();
    return ret;
}
#endif

class Sieve : public Generator<uint32_t>
{
private:
    uint8_t *_sieve;
    uint32_t _limit;
    uint32_t _next;
    uint32_t _findNext();
public:
    Sieve(uint32_t limit);
    virtual ~Sieve() { delete[] _sieve; }
    bool hasNext() { return _next > 0; }
    uint32_t next() { uint32_t ret = _next; _next = _findNext(); return ret; }
};

uint32_t Sieve::_findNext()
{   for (uint32_t i = _next + 1; i < _limit; i++)
        if (_sieve[i]) return i;
    return 0;
}

Sieve::Sieve(uint32_t limit)
{   _limit = limit;
    _sieve = new uint8_t[limit];
    for (uint32_t i = 0; i < limit; i++) _sieve[i] = 1;
    _sieve[0] = _sieve[1] = 0;
    for (uint32_t p = 2; p * p < limit; p++)
        if (_sieve[p] == 1)
            for (uint32_t i = p * 2; i < limit; i += p)
                _sieve[i] = 0;
    _next = 2;
}

template <typename T> static T triangle(T n) { return n * (n + 1) >> 1; }
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
                xswap(b[0], b[1]);
}

template <typename T, typename U> bool hasDigitsOnce2(T n, U nsetbeg, U nsetend)
{   while (n)
    {   uint32_t pos = linSearch(nsetbeg, nsetend, n % 10);
        if (pos) nsetbeg[pos - 1] = 99; else return false;
        n = n / 10;
    }
    return true;
}

class LongNumber25
{
private:
    uint8_t _buf[1500];
public:
    uint8_t *begin() { return _buf; }
    uint8_t *end() { return _buf + 1500; } // moet eigenlijk echte einde aangeven
    void set(uint64_t n)
    {   xmemset(_buf, 0, sizeof(_buf));
        for (uint16_t i = 0; n > 0; i++)
            _buf[i] = n % 10, n = n / 10;
    }
    void set(const LongNumber25 n)
    {   for (uint16_t i = 0; i < 1500; i++)
            _buf[i] = n.decimal(i);
    }
    void setdig(uint8_t pos, uint8_t n)
    {   _buf[pos] = n;
    }
    LongNumber25() { set(0); }
    LongNumber25(uint64_t n) { set(n); }
    LongNumber25(const LongNumber25 &n)
    {   for (uint16_t i = 0; i < 1500; i++) _buf[i] = n._buf[i];
    }
    uint16_t digits() const
    {   uint16_t i;
        for (i = 1500; i > 0; i--)
            if (_buf[i - 1] > 0)
                return i;
        return 1;
    }
    uint8_t decimal(uint16_t i) const { return _buf[i]; }
    void add(const LongNumber25 &n)
    {   uint8_t carry = 0;
        for (uint16_t i = 0; i < 1500; i++)
        {   _buf[i] += carry + n.decimal(i);
            carry = _buf[i] / 10;
            _buf[i] = _buf[i] % 10;
        }
    }
    void dec(LongNumber25 n)
    {
        if (lteq(n))
        {   set(0);
            return;
        }
        uint16_t carry = 0;
        for (uint16_t i = 0; i < 1500; i++)
        {   if (_buf[i] >= n._buf[i] + carry)
            {
                _buf[i] -= n._buf[i] + carry;
                carry = 0;
            }
            else
            {
                _buf[i] = 10 - (n._buf[i] - _buf[i]);
                carry = 1;
            }
        }
    }
    void dump(ostream &os) const
    {   for (uint16_t i = digits(); i > 0; i--)
            os << (uint16_t)_buf[i - 1];
    }
    bool equals(const LongNumber25 n) const
    {   for (uint16_t i = 0; i < 1500; i++)
            if (_buf[i] != n._buf[i])
                return false;
        return true;
    }
    bool gt(const LongNumber25 n) const
    {   for (uint16_t i = 1; i <= 1500; i++)
        {   if (_buf[1500 - i] > n._buf[1500 - i]) return true;
            if (_buf[1500 - i] < n._buf[1500 - i]) return false;
        }
        return false;
    }
    bool gteq(const LongNumber25 n) const
    {   return gt(n) || equals(n);
    }
    bool lt(const LongNumber25 n) const
    {   return gteq(n) == false;
    }
    bool lteq(const LongNumber25 n) const
    {   return lt(n) || equals(n);
    }
    void shiftup()
    {
        uint16_t ln = digits();
        for (uint16_t i = ln; i > 0; i--)
            _buf[i] = _buf[i - 1];
        _buf[0] = 0;
    }
    void shiftup(uint16_t n)
    {   for (uint16_t i = 0; i < n; i++)
            shiftup();
    }
private:
    void _div(LongNumber25 d)
    {   LongNumber25 i(0);
        while (gteq(d))
        {   dec(d);
            i.add(LongNumber25(1));
        }
        set(i);
    }
public:
    void reverse2()
    {
#if 0
        for (uint16_t beg = 0, end = digits() - 1; beg <= end; beg++, end--)
            xswap(_buf[beg], _buf[end]);
#else
        LongNumber25 a;
        for (uint16_t i = digits(), j = 0; i > 0; i--, j++)
            a._buf[j] = _buf[i - 1];
        set(a);
#endif
    }
    void mul3(uint64_t n)
    {
        if (n == 0)
        {
            set(0);
            return;
        }
        if (n == 1)
            return;
        uint64_t carry = 0;
        uint64_t i;
        for (i = 0; i < 1500; i++)
        {
            carry += _buf[i] * n;
            _buf[i] = carry % 10;
            carry /= 10;
        }
        while (carry > 0)
        {
            _buf[i++] = carry % 10;
            carry /= 10;
        }
    }
    void mul2(const LongNumber25 n)
    {
        LongNumber25 tmp;
        LongNumber25 sum(0);
        for (uint16_t i = 0; i < n.digits(); i++)
        {   tmp.set(*this);
            tmp.mul3(n._buf[i]);
            tmp.shiftup(i);
            sum.add(tmp);
        }
        set(sum);
    }
    void divmod(LongNumber25 &div, LongNumber25 &mod, const LongNumber25 d) const
    {
        mod.set(*this);
        LongNumber25 ret(0);
        LongNumber25 tmp1(0);
        LongNumber25 tmp2;
        for (uint16_t i = mod.digits(); i > 0; i--)
        {
            tmp1.shiftup();
            tmp1.add(mod._buf[i - 1]);
            tmp2.set(tmp1);
            tmp2._div(d);
            if (tmp2.gt(0))
            {
                ret._buf[i - 1] = tmp2._buf[0];
                tmp1.set(d);
                tmp1.mul2(tmp2);
                for (uint16_t j = 1; j < i; j++)
                    tmp1.shiftup();
                mod.dec(tmp1);
                i = mod.digits() + 1;
                tmp1.set(0);
            }
        }
        div.set(ret);
        
    }
    void longdiv(const LongNumber25 d)
    {
        LongNumber25 div, mod;
        divmod(div, mod, d);
        set(div);
    }
    void mod(const LongNumber25 d)
    {
        LongNumber25 div, mod;
        divmod(div, mod, d);
        set(mod);
    }
};

static void myAssert(bool a, bool b, const char *msg)
{
    if (a != b)
        cout << "Error: " << msg << "\r\n";
}

static void testLongNum()
{
    LongNumber25 x(123456789);
    x.dec(30000000);
    myAssert(x.digits() == 8, true, "Error digits 1");
    myAssert(x.equals(93456789), true, "Error Decrement 1");
    x.set(10);
    x.shiftup();
    myAssert(x.equals(100), true, "Error shiftup 1");
    x.set(123);
    x.shiftup(3);
    myAssert(x.equals(123000), true, "Error shiftup 2");
    x.set(11);
    myAssert(x.gt(10), true, "Error GreaterThan 1");
    myAssert(x.gt(11), false, "Error Greater Than 2");
    myAssert(x.gt(12), false, "Error Greater Than 3");
    x.dec(11);
    myAssert(x.equals(0), true, "Error Decrement 2");
    x.set(20);
    x.mod(10);
    myAssert(x.equals(0), true, "Error Mod 1");
    x.set(19);
    myAssert(x.gt(20), false, "Error GreaterThan 4");
    myAssert(x.gteq(20), false, "Error GreaterEqual 1");
    myAssert(x.lt(20), true, "Error LesserThan 1");
    x.dec(20);
    myAssert(x.equals(0), true, "Error Decrement 3");
    x.set(50);
    x.dec(6);
    myAssert(x.equals(44), true, "Error Decrement 4");
    x.set(123456789);
    x.mod(100);
    myAssert(x.equals(89), true, "Error Mod 2");
    x.set(123456789);
    x.longdiv(100);
    myAssert(x.equals(1234567), true, "Error Div 1");
    x.set(150);
    x.longdiv(140);
    myAssert(x.equals(1), true, "Error LongDiv 1");
    x.set(300);
    x.longdiv(140);
    myAssert(x.equals(2), true, "Error LongDiv 2");
    x.set(3000);
    x.longdiv(140);
    myAssert(x.equals(21), true, "Error LongDiv 3");
    x.set(404);
    x.longdiv(2);
    myAssert(x.equals(202), true, "Error LongDiv 4");
    x.set(123456789);
    x.reverse2();
    myAssert(x.equals(987654321), true, "Error reverse 1");
    x.set(8118);
    x.reverse2();
    myAssert(x.equals(8118), true, "Error reverse 2");
    x.set(123456789);
    x.mul2(123456789);
    myAssert(x.equals(15241578750190521ULL), true, "Error mul 1");
    x.set(999);
    x.mul2(999);
    myAssert(x.equals(998001), true, "Error mul 2");
}

template <typename T> uint16_t decimals(T n)
{   uint16_t i = 0;
    while (n) n /= 10, i++;
    return i;
}

template <> uint16_t decimals<LongNumber25>(LongNumber25 n)
{   return n.digits();
}

template <typename T> bool sameDigs(T a, T b)
{   uint16_t decs = decimals(a), end = 0;
    uint8_t *nset = new uint8_t[decs];
    for (;a ;a = a / 10) nset[end++] = a % 10;
    bool ret = hasDigitsOnce2<T>(b, nset, nset + end);
    delete[] nset;
    return ret;
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
    uint16_t _kounter3[50];
    uint8_t _end;
public:
    Kounter() : _end(0) { }
    void insert(uint8_t n);
    bool hasKey(uint8_t n);
    uint8_t maxKey();
    uint8_t maxCount();
    uint8_t minKey();
    uint8_t minCount();
    uint16_t *begin() { return _kounter3; }
    uint16_t *end() { return _kounter3 + _end; }
};

bool Kounter::hasKey(uint8_t n)
{   for (uint16_t *it = begin(); it != end(); it++)
        if ((uint8_t)(*it >> 8) == n) return true;
    return false;
}

uint8_t Kounter::maxKey()
{   uint8_t xmax = 0;
    for (uint8_t i = 0; i < _end; i++)
        xmax = std::max(xmax, (uint8_t)((_kounter3[i] >> 8) & 0xff));
    return xmax;
}

uint8_t Kounter::maxCount()
{   uint8_t xmax = 0;
    for (uint16_t *it = begin(); it != end(); it++)
        xmax = std::max(xmax, (uint8_t)(*it & 0xff));
    return xmax;
}

uint8_t Kounter::minKey()
{   uint8_t xmin = 0xff;
    for (uint16_t *it = begin(); it != end(); it++)
        xmin = std::min(xmin, (uint8_t)((*it >> 8) & 0xff));
    return xmin;
}

uint8_t Kounter::minCount()
{   uint8_t xmin = 0xff;
    for (uint16_t *it = begin(); it != end(); it++)
        xmin = std::min(xmin, (uint8_t)(*it & 0xff));
    return xmin;
}

void Kounter::insert(uint8_t n)
{   for (uint8_t i = 0; i < _end; i++)
    {   if ((_kounter3[i] & 0xff00) == (uint16_t)n << 8)
        { _kounter3[i]++; return; }
    }
    _kounter3[_end++] = (uint16_t)n << 8 | 1;
}

template <class T> class PrimeFactors2 : public Generator<uint64_t>
{
private:
    T _begin;
    T _end;
    uint64_t _n;
public:
    PrimeFactors2(T begin, T end, uint64_t n) : _begin(begin), _end(end), _n(n) { }
    bool hasNext() { return _n > 1; }
    uint64_t next()
    {   uint64_t factor = 0;
        for (T it = _begin; it != _end; it++)
        {   if (_n % *it == 0)
            {   factor = *it;
                break;
            }
        }
        _n = _n / factor;
        return factor;
    }
};

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

template <typename T> static uint32_t sumDivs2(T begin, T end, uint32_t n)
{
    PrimeFactors2<T> gen(begin, end, n);
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

template <typename T> static uint32_t sumProperDivs2(T begin, T end, uint32_t n)
{   return sumDivs2(begin, end, n) - n;
}

static bool isPandigital41(uint32_t n)
{
    uint16_t decs = decimals(n);
    uint8_t *nset = new uint8_t[decs];
    for (uint8_t i = 1; i <= decs; i++) nset[i - 1] = i;
    bool ret = hasDigitsOnce2(n, nset, nset + decs);
    delete[] nset;
    return ret;
}

template <typename T> static T gcd(T a, T b)
{   while (b)
    {   T c = b;
        b = a % b;
        a = c;
    }
    return a;
}
#if 0
static uint32_t gcd(uint32_t a, uint32_t b)
{   while (b)
    {   uint32_t c = b;
        b = a % b;
        a = c;
    }
    return a;
}
#endif

static uint32_t ways32(uint32_t target, uint32_t *begin, uint32_t *end)
{
    uint32_t *lst = new uint32_t[target + 1];
    for (uint32_t i = 0; i <= target; i++) lst[i] = 0;
    lst[0] = 1;
    for (uint32_t *it = begin; it != end; it++)
        for (uint32_t i = *it; i < target + 1; i++)
            lst[i] += lst[i - *it];
    uint32_t ret = lst[target];
    delete[] lst;
    return ret;
}

static double xabs(double n)
{   return n < 0 ? n *= -1 : n;
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

template <typename T> struct MyArray
{
    T *arr;
    uint32_t size;
    MyArray(uint32_t xsize) : size(xsize) { arr = new T[size]; }
    MyArray(T *begin, T *end)
    {   size = end - begin;
        arr = new T[size];
        for (uint32_t i = 0; i < size; i++)
            arr[i] = begin[i];
    }
    void operator=(const MyArray &ma)
    {   size = ma.size;
        arr = new T[size];
        for (uint32_t i = 0; i < size; i++)
            arr[i] = ma.arr[i];
    }
    MyArray(const MyArray &ma)
    {   size = ma.size;
        arr = new T[size];
        for (uint32_t i = 0; i < size; i++)
            arr[i] = ma.arr[i];
    }
    ~MyArray() { delete[] arr; }
};

template <typename T> class Permutation
{
private:
    T *_pool;
    uint8_t _size;
    T *_c;
    uint32_t _i;
    bool _hasNext;
public:
    Permutation(T *begin, T *end) : _i(0), _hasNext(true)
    {
        _size = end - begin;
        _pool = new T[_size];
        _c = new T[_size];
        for (uint8_t j = 0; j < _size; j++) _c[j] = 0;
        uint32_t i = 0;
        while (begin != end)
            _pool[i++] = *begin++;
    }
    ~Permutation()
    {   delete[] _pool;
        delete[] _c;
    }
    bool hasNext()
    {
        return _hasNext;;
    }
    MyArray<T> next()
    {
        MyArray<T> ret(_pool, _pool + _size);
        while (_i < _size)
        {   if (_c[_i] < _i)
            {   if (_i % 2 == 0)
                    xswap(_pool[0], _pool[_i]);
                else
                    xswap(_pool[_c[_i]], _pool[_i]);
                _c[_i]++, _i = 0;
                return ret;
            }
            else _c[_i++] = 0;
        }
        _hasNext = false;
        return ret;
    }
};

template <typename T> class Combination
{
private:
};

template <class T> class Polygonizer2 : public Generator<T>
{
private:
    T _step;
    T _ret;
    bool _hasNext;
    T _limit;
    T _n;
    T _xmin;
public:
    Polygonizer2(T limit, T n = 1, T xmin = 0) :
        _step(1), _ret(0), _hasNext(true), _limit(limit), _n(n), _xmin(xmin) { }
    bool hasNext() { return _hasNext; }
    T next()
    {
        T ret;
        while (true)
        {
            ret = _ret;
            _ret += _step;
            _step += _n;
            if (_ret >= _limit) _hasNext = false;
            if (ret >= _xmin) break;
        }
        return ret;
    }
};

#if 0
class Digits : public Generator<uint8_t>
{
private:
    uint64_t _n;
public:
    Digits(uint64_t n) : _n(n) { }
    bool hasNext() { return _n > 0; }
    uint8_t next()
    {
        uint8_t ret = _n % 10;
        _n = _n / 10;
        return ret;
    }
};
#endif

template <typename T> static bool ispolygon(T n, T size)
{   Polygonizer2<T> p(n + 1, size);
    return last(p) == n;
}

template <typename T> static bool issquare(T n)
{   return ispolygon<T>(n, 2);
}

static bool _issquare3(uint64_t n, uint64_t lower, uint64_t upper)
{
    uint64_t range2 = upper - lower;
    uint64_t root = lower + range2 / 2;
    if (root * root == n) return true;
    if (lower == upper) return false;
    if (root * root > n) return _issquare3(n, lower, root);
    return _issquare3(n, root + 1, upper);
}

static bool issquare3(uint64_t n)
{
    return _issquare3(n, 1, n - 1);
}

static uint64_t ceildiv(uint64_t a, uint64_t b)
{   uint64_t ret = a / b;
    return ret * b == a ? ret : ret + 1;
}

template <typename T> static T floorsqrt(T n)
{   Polygonizer2<T> p(n + 1, 2);
    T ret = 0;
    while (p.hasNext())
    {   p.next();
        ret++;
    }
    return ret - 1;
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
    xmemset(largeNum, 0, 400);
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
    return xstrlen(s);
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

static uint8_t triangle18[][15] = {
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

static string problem18()
{   uint32_t possibilities = myPow<uint64_t>(2, sizeof(triangle18[0]) - 1);
    uint32_t best = 0;
    for (uint32_t i = 0; i <= possibilities; i++)
    {   uint32_t index = 0;
        uint32_t sum = triangle18[0][0];
        for (uint32_t j = 0; j < sizeof(triangle18[0]) - 1; j++)
        {   index = index + (i >> j & 1);
            uint8_t value = triangle18[j + 1][index];
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
    for (uint16_t year = 1901; year <= 2000; year++)
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
    xmemset(buf, 0, 400);
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
    xmemset(l, 0, (high - low) * 4);
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

static uint8_t letterwaarde(uint8_t c)
{   return c > 64 ? c - 64 : c;
}

static void swap22(char *a, char *b)
{   char tmp = *a;
    *a = *b;
    *b = tmp;
}

static string problem22()
{   FILE *fp;
    fp = fopen("euler22.txt", "r");
    char *names = new char[30*6000];
    xmemset(names, 0, 30*6000);
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
            if (xstrcmp(names + b * 30, names + (b + 1) * 30) > 0)
                for (i = 0; i < 30; i++)
                    swap22(names + b * 30 + i, names + (b + 1) * 30 + i);
    uint32_t total = 0;
    for (a = 0; a < 5163; a++)
    {   uint32_t score = 0;
        for (i = 0; i < 30; i++) score += letterwaarde(names[a * 30 + i]);
        score = score * (a + 1);
        total += score;
    }
    delete[] names;
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

static string problem23()
{   uint16_t xmax = 28123;
    uint32_t *lprimes = new uint32_t[9999], end = 0, end2 = 0;
    Sieve sieve(99999);
    while (sieve.hasNext())
        lprimes[end++] = sieve.next();
    uint16_t *abundants = new uint16_t[28123];
    for (uint16_t i = 1; i <= xmax; i++)
        if (sumProperDivs2(lprimes, lprimes + end, i) > i)
            abundants[end2++] = i;
    uint32_t xsum = 1;
    for (uint16_t i = 2; i <= xmax; i++)
    {   bool boo = true;
        for (uint16_t *it = abundants; it != abundants + (end2 - 1); it++)
        {   if (*it < i)
            {
                if (binSearch(abundants, abundants + (end2 - 1), i - *it))
                {   boo = false;
                    break;
                }
            } else break;
        }
        if (boo == true) xsum += i;
    }
    delete[] lprimes;
    delete[] abundants;
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
    fib[0].set(1);
    fib[1].set(0);
    fib[2].set(1);
    while (decimals(fib[i]) < 1000)
    {   i = (i + 1) % 3;
        cnt++;
        LongNumber25 tmp(fib[(i + 1) % 3]);
        tmp.add(fib[(i + 2) % 3]);
        fib[i].set(tmp);
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

static string problem29()
{   uint32_t buf[99*99];
    for (uint16_t a = 2; a <= 100; a++)
        for (uint16_t b = 2; b <= 100; b++)
            buf[(a-2)*99+(b-2)] = eqpow(a << 16 | b);
    bubbleSort(buf, buf + 99*99);
    return twostring(ndistinct(buf, buf + 99*99));
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
{
    uint32_t target = 200, coins[] = {1,2,5,10,20,50,100,200};
    return twostring<uint32_t>(ways32(target, coins, coins + 8));
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

static uint32_t panProducts32(uint32_t *st)
{   uint32_t ret = 0;
    for (uint32_t i = 2; i < 60; i++)
    {   uint32_t start = i < 10 ? 1234 : 123;
        for (uint32_t j = start; j < 10000/i; j++)
        {   uint8_t nset[9];
            for (uint8_t n = 1; n <= 9; n++) nset[n - 1] = n;
            if (hasDigitsOnce2(i, nset, nset + 9) == false) continue;
            if (hasDigitsOnce2(j, nset, nset + 9) == false) continue;
            if (hasDigitsOnce2(i * j, nset, nset + 9)  == false) continue;
            st[ret++] = i * j;
        }
    }
    return ret;
}

static string problem32()
{   uint32_t st[100];
    uint32_t end = panProducts32(st);
    bubbleSort(st, st + end);
    uint32_t xsum = 0, previous = 0;
    for (uint32_t *it = st; it != st + end; it++)
    {   if (*it != previous)
            xsum += *it;
        previous = *it;
    }
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
{   ifstream ifs;
    ifs.open("euler42.txt");
    uint32_t triangles[20], ret = 0;
    for (uint32_t i = 0; i < 20; i++) triangles[i] = triangle32(i);
    string tmp;
    while (getline(ifs, tmp))
    {   if (tmp.size() > 0)
        {   if (linSearch(triangles, triangles + 20, wordcount42(tmp.c_str())))
                ret++;
        }
    }
    ifs.close();
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

/*
1,460,357,289 + 4,130,952,867 + 4,106,357,289 + 1,430,952,867 +
1,406,357,289 + 4,160,357,289 = 16,695,334,890
*/

static uint64_t concat43(uint8_t *beg)
{   uint64_t ret = 0;
    for (uint32_t i = 0; i <= 9; i++)
        ret += beg[9 - i] * myPow<uint64_t>(10, i);
    return ret;
}

static bool test43(uint64_t n)
{   uint8_t divs[] = {17,13,11,7,5,3,2};
    for (uint8_t i = 0; i < 7; i++)
        if ((n / myPow<uint32_t>(10, i) % 1000) % divs[i] != 0)
            return false;
    return true;
}

static string problem43()
{   uint8_t pool[] = {0,1,2,3,4,5,6,7,8,9};
    uint64_t xsum = 0;
    Permutation<uint8_t> p(pool, pool + 10);
    while (p.hasNext())
    {   MyArray<uint8_t> ma = p.next();
        uint64_t ccat = concat43(ma.arr);
        xsum += test43(ccat) ? ccat : 0;
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
            if (binSearch(lpgs + j, lpgs + 9997, lpgs[i] + lpgs[j]) &&
                binSearch(lpgs, lpgs + 9997, lpgs[j] - lpgs[i]))
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

static uint64_t
pair46(uint32_t *primbeg, uint32_t *primend, uint32_t *sqbeg, uint32_t *sqend, uint32_t n)
{   for (uint32_t *it = primbeg; it != primend; it++)
    {   if (*it > n) break;
        if (binSearch(sqbeg, sqend, n - *it))
            return (uint64_t)*it << 32 | (n - *it);
    }
    return 0;
}

static uint32_t opdracht46()
{   uint32_t squares[100], *primes = new uint32_t[80000], end = 0;
    Sieve sieve(999999);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    end--;
    uint32_t i;
    for (i = 0; i < 100; i++) squares[i] = 2*i*i;
    for (i = 3; i < 987654321; i += 2)
    {   if (binSearch(primes, primes + end, i)) continue;
        uint64_t pr = pair46(primes, primes + end, squares, squares + 100, i);
        if (pr == 0) break;
    }
    delete[] primes;
    return i;
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
    xmemset(f, 0, 4*L);
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

The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.

Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.

Antwoord: 9,110,846,700
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
    delete[] primes;
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

template <typename T> static uint32_t
family51(uint32_t *first, T beg, T end, uint32_t n, uint32_t mask)
{   uint32_t xlen = decimals<uint32_t>(n);
    uint32_t bmask2[64], div = 2, dec = 1, end2 = 0;
    for (uint32_t n2 = mask; n2;)
    {   bmask2[end2++] = n2 % div ? dec : 0;
        n2 -= n2 % div;
        div *= 2;
        dec *= 10;
    }
    for (uint32_t i = 0; i < end2; i++)
        n -= bmask2[i] * digit<uint32_t>(n, i);
    uint32_t size = 0;
    for (uint32_t i = 0; i < 10; i++)
    {   uint32_t tmp = n;
        for (uint32_t *it = bmask2; it != bmask2 + end2; it++)
            tmp += *it * i;
        if (decimals<uint32_t>(tmp) == xlen && binSearch(beg, end - 1, tmp))
        {   size++;
            if (size == 1) *first = tmp;
        }
    }
    return size;
}

static uint32_t opdracht51()
{   uint32_t primes[80000], end = 0, first = 0;
    Sieve sieve(1000000);
    while (sieve.hasNext()) primes[end++] = sieve.next();
    for (uint32_t *it = primes; it != primes + end; it++)
    {   for (uint32_t mask = 1; mask < myPow<uint32_t>(2, decimals<uint32_t>(*it)); mask++)
        {   uint32_t size = family51(&first, primes, primes + end, *it, mask);
            if (size == 8) return first;
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
{   uint8_t nset[10], end = 0;;
    for (uint32_t x = n; x; x = x / 10)
        nset[end++] = x % 10;
    for (uint32_t m = 2; m <= 6; m++)
    {
        uint8_t nset2[10];
        xmemcpy(nset2, nset, end);
        if (hasDigitsOnce2(n * m, nset2, nset2 + end) == false) return false;
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

static uint8_t straight(Kounter &cnt)
{   uint8_t xmax = cnt.maxKey();
    for (uint8_t i = 0; i < 5; i++)
        if (cnt.hasKey(xmax - i) == false) return 0;
    return xmax;
}

static bool flush(Kounter &cnt)
{   return cnt.maxCount() == 5;
}

static uint32_t fullHouse(Kounter &cnt)
{   return cnt.maxCount() == 3 && cnt.minCount() == 2;
}

static uint32_t fourkind(Kounter &cnt)
{   return cnt.maxCount() == 4;
}

static uint32_t threekind(Kounter &cnt)
{   return cnt.maxCount() == 3 && cnt.minCount() == 1;
}

static uint8_t twopair(Kounter &cnt)
{   uint8_t best = 0;
    uint8_t pairCount = 0;
    for (uint16_t *it = cnt.begin(); it != cnt.end(); it++)
    {   if ((uint8_t)(*it & 0xff) == 2)
        {   pairCount++;
            best = max(best, (uint8_t)(*it >> 8));
        }
    }
    return pairCount == 2 ? best : 0;
}

static uint8_t onepair(Kounter &cnt)
{   uint8_t bestKicker = 0, pairValue = 0, pairCount = 0;
    for (uint16_t *it = cnt.begin(); it != cnt.end(); it++)
    {
        if ((uint8_t)(*it & 0xff) == 2)
        {   pairCount++;
            pairValue = (uint8_t)((*it >> 8) & 0xff);
        }
        else if ((uint8_t)(*it & 0xff) == 1)
        {   bestKicker = max(bestKicker, (uint8_t)((*it >> 8) & 0xff));
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
    return values.maxKey();   // high card
}

static string problem54()
{   ifstream ifs;
    ifs.open("euler54.txt");
    string tmp;
    uint32_t ret = 0;
    while (getline(ifs, tmp))
    {   if (tmp.size() > 0)
        {   uint64_t hand = parse(tmp);
            if (score(hand & 0xffffffff) > score((hand & 0xffffffff00000000ULL) >> 32))
                ret++;
        }
    }
    ifs.close();
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
    {
        LongNumber25 rev(n);
        rev.reverse2();
        n.add(rev);
        rev.set(n);
        rev.reverse2();
        if (n.equals(rev)) return 0;
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

static string problem56()
{   uint32_t maximum = 100, maxSum = 1;
    for (uint32_t base = 1; base <= maximum; base++)
    {   LongNumber25 power(1);
        for (uint32_t exponent = 1; exponent <= maximum; exponent++)
        {   uint32_t sum = 0;
            for (uint8_t *it = power.begin(); it != power.end(); it++)
                sum += *it;
            maxSum = max(maxSum, sum);
            power.mul3(base);
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
    LongNumber25 a(1);
    LongNumber25 b(1);
    uint32_t count = 0;
    for (uint32_t i = 0; i <= iterations; i++)
    {   if (a.digits() > b.digits()) count++;
        LongNumber25 twoB(b);
        twoB.add(b);
        LongNumber25 nextA(a);
        nextA.add(twoB);
        LongNumber25 nextB(b);
        nextB.add(a);
        a.set(nextA);
        b.set(nextB);
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

template <typename T, typename U> static void decipher(T inbeg, T inend, U outbeg, uint32_t key2)
{   uint8_t key[3];
    key[0] = (key2 & 0xff000000) >> 24;
    key[1] = (key2 & 0xff0000) >> 16;
    key[2] = (key2 & 0xff00) >> 8;
    uint32_t i = 0;
    for (; inbeg != inend; i++, inbeg++, outbeg++)
        *outbeg = *inbeg ^ key[i % 3];
    outbeg[i] = 0;
}

static double english[] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.0153, 0.0772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095,
    0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

static uint32_t analysis(char *beg, char *end, uint32_t *letters)
{   uint32_t total = 0;
    for (uint8_t i = 0; i < 26; i++) letters[i] = 0;
    while (beg != end)
    {   if (isalpha(*beg))
        {   total++;
            char low = tolower(*beg);
            letters[low - 'a']++;
        }
        beg++;
    }
    return total;
}

static string problem59()
{
    // read message from file
    uint8_t msg[1400];
    ifstream ifs;
    ifs.open("euler59.txt");
    char c;
    uint8_t n = 0;
    uint32_t end = 0;
    while (ifs.get(c))
    {   if (isdigit(c) == false)
        {   msg[end++] = n;
            n = 0;
            continue;
        }
        n = n * 10 + (c - 48);
    }
    ifs.close();

    // generate keys
    uint32_t *keys = new uint32_t[26*26*26];
    for (uint8_t i = 0; i < 26; i++)
        for (uint8_t j = 0; j < 26; j++)
            for (uint8_t k = 0; k < 26; k++)
                keys[i * 26 * 26 + j * 26 + k] = (i + 97) << 24 | (j + 97) << 16 | (k + 97) << 8;

    char output[1400];
    double best_sumdif = 999999.9;
    uint32_t best_key = 0;
    for (uint32_t *it = keys; it != keys + 26 * 26 * 26; it++)
    {   decipher(msg, msg + end, output, *it);
        uint32_t letters[26];
        uint32_t total = analysis(output, output + end, letters);
        double sumdif = 0;
        for (uint8_t i = 0; i < 26; i++)
        {   double relative = (double)letters[i] / (double)total;
            sumdif += xabs(relative - english[i]);
        }
        if (sumdif < best_sumdif)
            best_sumdif = sumdif, best_key = *it;
    }
    delete[] keys;
    uint32_t xsum = 0;
    decipher(msg, msg + end, output, best_key);
    for (char *it = output; it != output + end; it++) xsum += *it;
    return twostring<uint32_t>(xsum);
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

static bool comb(uint32_t a, uint32_t b)
{   return isPrime(a * myPow<uint32_t>(10, decimals(b)) + b) &&
        isPrime(b * myPow<uint32_t>(10, decimals(a)) + a);
}

static uint32_t opdracht60()
{   uint32_t lprimes[10000], end = 0;
    Sieve sieve(10000);
    while (sieve.hasNext())
        lprimes[end++] = sieve.next();
    for (uint32_t *ita = lprimes; ita != lprimes + end; ita++)
    {   for (uint32_t *itb = lprimes; itb != lprimes + end; itb++)
        {   if (*itb < *ita) continue;
            if (comb(*ita, *itb))
            {   for (uint32_t *itc = lprimes; itc != lprimes + end; itc++)
                {   if (*itc < *itb) continue;
                    if (comb(*ita, *itc) && comb(*itb, *itc))
                    {   for (uint32_t *itd = lprimes; itd != lprimes + end; itd++)
                        {   if (*itd < *itc) continue;
                            if (comb(*ita, *itd) && comb(*itb, *itd) && comb(*itc, *itd))
                            {   for (uint32_t *ite = lprimes; ite != lprimes + end; ite++)
                                {   if (*ite < *itd) continue;
                                    if (comb(*ita, *ite) && comb(*itb, *ite) &&
                                        comb(*itc, *ite) && comb(*itd, *ite))
                                    {
                                        return *ita + *itb + *itc + *itd + *ite;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

static string problem60()
{   return twostring<uint32_t>(opdracht60());
}

/*
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
*/

/*
8128 (Hex) + 2882 (Pent) + 8256 (Tri) + 5625 (Sq) + 2512 (Hept) + 1281 (Oct) = 28,684
*/

static uint32_t opdracht61()
{   uint8_t perms[4320], pool[] = {0,1,2,3,4,5}, c[6] = {0};
    uint16_t i16 = 6;
    xmemset(perms, 0, 4320);
    xmemcpy(perms, pool, 6);
    for (uint8_t i = 0, size = 6, tmp = 0; i < size;)
    {   if (c[i] < i)
        {   if (i % 2 == 0) tmp = pool[0], pool[0] = pool[i], pool[i] = tmp;
            else tmp = pool[c[i]], pool[c[i]] = pool[i], pool[i] = tmp;
            c[i]++, i = 0;
            xmemcpy(perms + i16, pool, 6);
            i16 += 6;
        }
        else c[i++] = 0;
    }
    for (uint16_t i = 0; i < 720; i++)
    {   Polygonizer2<uint32_t> p0(9999, perms[i * 6] + 1, 1000);
        while (p0.hasNext())
        {   uint32_t n0 = p0.next();
            Polygonizer2<uint32_t> p1(9999, perms[i * 6 + 1] + 1, 1000);
            while (p1.hasNext())
            {   uint32_t n1 = p1.next();
                if (n1 / 100 == n0 % 100)
                {   Polygonizer2<uint32_t> p2(9999, perms[i * 6 + 2] + 1, 1000);
                    while (p2.hasNext())
                    {   uint32_t n2 = p2.next();
                        if (n2 / 100 == n1 % 100)
                        {   Polygonizer2<uint32_t> p3(9999, perms[i * 6 + 3] + 1, 1000);
                            while (p3.hasNext())
                            {   uint32_t n3 = p3.next();
                                if (n3 / 100 == n2 % 100)
                                {   Polygonizer2<uint32_t> p4(9999, perms[i * 6 + 4] + 1, 1000);
                                    while (p4.hasNext())
                                    {   uint32_t n4 = p4.next();
                                        if (n4 / 100 == n3 % 100)
                                        {   Polygonizer2<uint32_t> p5(9999,
                                                perms[i * 6 + 5] + 1, 1000);
                                            while (p5.hasNext())
                                            {   uint32_t n5 = p5.next();
                                                if (n5 / 100 == n4 % 100)
                                                {   if (n5 % 100 == n0 / 100)
                                                    {   return n0 + n1 + n2 + n3 + n4 + n5;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
    return 0;
}

static string problem61()
{   return twostring<uint32_t>(opdracht61());
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
{   uint64_t lst[9000];
    for (uint64_t n = 0; n < 9000; n++)
        lst[n] = n * n * n;
    for (uint32_t i = 0; i < 9000; i++)
    {   uint32_t cnt = 0;
        uint8_t ln = decimals<uint64_t>(lst[i]);
        for (uint32_t b = i; b < 9000; b++)
        {   if (decimals(lst[b]) > ln) break;
            if (sameDigs<uint64_t>(lst[i], lst[b])) cnt++;
        }
        if (cnt == 5)
            return twostring<uint64_t>(lst[i]);
    }
    return twostring<uint64_t>(0);
}

/*
#63: Powerful digit counts

The 5-digit number, 16807=7^5, is also a fifth power. Similarly,
the 9-digit number, 134217728=8^9, is a ninth power.

How many n-digit positive integers exist which are also an nth power?

Antwoord: 49
*/

/*
1^1 ~ 9^1, 4^2 ~ 9^2, 5^3 ~ 9^3, 6^4 ~ 9^4, 7^5 ~ 9^5, 7^6 ~ 9^6,
8^7, 9^7, 8^8, 9^8, 8^9, 9^9, 8^10, 9^10, 9^11, 9^12, 9^13, 9^14,
9^15, 9^16, 9^17, 9^18, 9^19, 9^20, 9^21
*/

//https://stackoverflow.com/questions/35968963/
//trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having
static const double LN10 = 2.3025850929940456840179914546844;

static double xln(double x)
{   double old_sum = 0.0, xmlxpl = (x - 1) / (x + 1);
    double xmlxpl_2 = xmlxpl * xmlxpl, denom = 1.0;
    double frac = xmlxpl;
    double term = frac;                 // denom start from 1.0
    double sum = term;
    while (sum != old_sum)
        old_sum = sum, denom += 2.0, frac *= xmlxpl_2, sum += frac / denom;
    return 2.0 * sum;
}

static double xlog10(double x)
{   return xln(x) / LN10;    
}

static uint32_t decipow(uint32_t base, uint32_t e)
{   return (uint32_t)(e * xlog10(base)) + 1;
}

static string problem63()
{   uint32_t xsum = 0;
    for (uint32_t e = 1; e < 99; e++)
    {   uint32_t subsum = 0;
        for (uint32_t base = 1; base < 10; base++)
            subsum += decipow(base, e) == e;
        xsum += subsum;
        if (subsum == 0) break;
    }
    return twostring<uint32_t>(xsum);
}

/*
#64: Odd period square roots

Antwoord: 1,322
*/

/*
https://blog.dreamshire.com/project-euler-64-solution/
*/

static string problem64()
{
    uint32_t L = 10000, odd_period = 0;
    for (uint32_t N = 2; N <= L; N++)
    {
        uint32_t r = floorsqrt<uint32_t>(N);
        uint32_t limit = floorsqrt<uint32_t>(N);
        if (limit * limit == N) continue;
        uint32_t k = 1, period = 0;
        while (k != 1 || period == 0)
        {   k = (N - r * r) / k;
            r = (limit + r) / k * k - r;
            period++;
        }
        if (period % 2 == 1) odd_period++;
    }
    return twostring<uint32_t>(odd_period);
}

/*
#65: Convergents of e

Antwoord: 272
*/

static string problem65()
{   uint32_t L = 100;
    LongNumber25 n0(1), n1(2);
    for (uint32_t i = 2; i <= L; i++)
    {   LongNumber25 tmp(n1);
        if (i % 3 > 0)
        {   n1.add(n0);
        }
        else
        {   LongNumber25 tmp2(n1);
            tmp2.mul3(2 * i/3);
            n1.set(n0);
            n1.add(tmp2);
        }
        n0.set(tmp);
    }
    uint32_t xsum = 0;
    for (uint8_t *it = n1.begin(); it != n1.end(); it++)
        xsum += *it;
    return twostring<uint32_t>(xsum);
}

/*
#66: Diophantine equation

Consider quadratic Diophantine equations of the form:

x^2 – Dy^2 = 1

For example, when D=13, the minimal solution in x is 649^2 - 13x180^2 = 1.

It can be assumed that there are no solutions
in positive integers when D is square.

By finding minimal solutions in x for
D = {2, 3, 5, 6, 7}, we obtain the following:

32 - 2x22 = 1
22 - 3x12 = 1
92 - 5x42 = 1
52 - 6x22 = 1
82 - 7x32 = 1

Hence, by considering minimal solutions in x for
D <= 7, the largest x is obtained when D=5.

Find the value of D <= 1000 in minimal solutions
of x for which the largest value of x is obtained.

Antwoord: 661
*/

static string problem66()
{   LongNumber25 x[3], y[3], tmp, tmp2, best_x;
    uint64_t a2, numerator2, denominator2;
    best_x.set(0);
    uint64_t best_d = 0;
    for (uint64_t d = 2; d <= 1000; d++)
    {   uint64_t root = floorsqrt<uint64_t>(d);
        if (root * root == d) continue;
        a2 = root;
        numerator2 = 0;
        denominator2 = 1;
        x[0].set(0);
        x[1].set(1);
        x[2].set(root);
        y[0].set(0);
        y[1].set(0);
        y[2].set(1);
        while (true)
        {   numerator2 = denominator2 * a2 - numerator2;
            denominator2 = (d - numerator2 * numerator2) / denominator2;
            a2 = (root + numerator2) / denominator2;
            x[0].set(x[1]);
            x[1].set(x[2]);
            x[2].set(x[1]);
            x[2].mul3(a2);
            x[2].add(x[0]);
            y[0].set(y[1]);
            y[1].set(y[2]);
            y[2].set(y[1]);
            y[2].mul3(a2);
            y[2].add(y[0]);
            tmp.set(x[2]);
            tmp.mul2(x[2]);
            tmp2.set(y[2]);
            tmp2.mul2(y[2]);
            tmp2.mul3(d);
            tmp2.add(1);
            if (tmp.equals(tmp2))
                break;
        }
        if (best_x.lt(x[2]))
        {   best_x.set(x[2]);
            best_d = d;
        }
    }
    return twostring<uint64_t>(best_d);
}

/*
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
altogether! If you could check one trillion (10^12) routes every second
it would take over twenty billion years to check them all. There is an
efficient algorithm to solve it. ;o)

Antwoord: 7,273
*/

static string problem67()
{   uint64_t arr[5050];
    ifstream ifs;
    ifs.open("euler67.txt");
    char c;
    uint16_t i = 0;
    uint8_t x = 0;
    while ((ifs.get(c)))
    {   if (isdigit(c))
            x = x * 10 + (c - '0');
        else if (x > 0)
            arr[i++] = x, x = 0;
    }
    ifs.close();
    uint64_t root = 100;
    while (root > 1)
    {   for (uint64_t j = 0; j < root - 1; j++)
        {   uint64_t k = triangle(root - 2) + j;
            uint64_t l = triangle(root - 1) + j;
            arr[k] += std::max(arr[l], arr[l + 1]);
        }
        root--;
    }
    return twostring<uint64_t>(arr[0]);
}

/*
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
*/

static bool sumEqual(uint8_t *lines)
{   uint8_t sum1 = lines[0] + lines[1] + lines[2];
    for (uint8_t i = 1; i <= 4; i++)
        if (lines[i * 3 + 0] + lines[i * 3 + 1] + lines[i * 3 + 2] != sum1)
            return false;
    return true;
}

static string problem68()
{   uint8_t pool[] = {1,2,3,4,5,6,7,8,9,10};
    Permutation<uint8_t> p(pool, pool + 10);
    uint8_t lines[15];
    uint64_t best = 0;
    while (p.hasNext())
    {   MyArray<uint8_t> ma = p.next();
        lines[0] = ma.arr[0], lines[1] = ma.arr[5], lines[2] = ma.arr[6];
        lines[3] = ma.arr[1], lines[4] = ma.arr[6], lines[5] = ma.arr[7];
        lines[6] = ma.arr[2], lines[7] = ma.arr[7], lines[8] = ma.arr[8];
        lines[9] = ma.arr[3], lines[10] = ma.arr[8], lines[11] = ma.arr[9];
        lines[12] = ma.arr[4], lines[13] = ma.arr[9], lines[14] = ma.arr[5];
        if (sumEqual(lines) == false) continue;
        if (ma.arr[0] > ma.arr[1]) continue;
        if (ma.arr[0] > ma.arr[2]) continue;
        if (ma.arr[0] > ma.arr[3]) continue;
        if (ma.arr[0] > ma.arr[4]) continue;
        uint64_t digstr = 0;
        for (uint8_t i = 0; i < 15; i++)
            digstr *= lines[i] == 10 ? 100 : 10, digstr += lines[i];
        if (decimals(digstr) != 16) continue;
        best = std::max(best, digstr);
    }
    return twostring<uint64_t>(best);
}

/*
#69: Totient maximum

Euler's Totient function, phi(n) [sometimes called the phi function], is
used to determine the number of numbers less than n which are relatively
prime to n. For example, as 1, 2, 4, 5, 7, and 8, are all less than nine
and relatively prime to nine, phi(9)=6.

Antwoord: 510,510
*/

static uint32_t opdracht69()
{
    uint32_t lprimes[100], nprimes = 0;
    Sieve sieve(100);
    while (sieve.hasNext()) lprimes[nprimes++] = sieve.next();
    uint32_t maxn = 1, L = 1000000;
    for (uint32_t i = 0; i < nprimes; i++)
    {
        if (maxn * lprimes[i] > L) return maxn;
        maxn *= lprimes[i];
    }
    return 0;
}

static string problem69()
{
    return twostring(opdracht69());
}

/*
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
*/

static uint64_t opdracht70(uint32_t L = 10000000)
{
    Sieve sieve(3800);
    uint32_t primes[600], nprimes = 0;
    while (sieve.hasNext()) primes[nprimes++] = sieve.next();
    uint64_t min_n = 0, i = 0;
    double min_q = 2;
    for (uint32_t *p1 = primes; p1 != primes + nprimes; p1++)
    {   i++;
        for (uint32_t *p2 = primes + i; p2 != primes + nprimes; p2++)
        {
            if ((*p1 + *p2) % 9 != 1) continue;
            uint64_t n = *p1 * *p2;
            if (n > L) return min_n;
            uint64_t phi = (*p1 - 1) * (*p2 - 1);
            double q = n / (double)phi;
            if (sameDigs(phi, n) && min_q > q)
                min_q = q, min_n = n;
        }
    }
    return 0;
}

static string problem70()
{   return twostring<uint64_t>(opdracht70());
}

/*
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
*/

static string problem71(uint64_t limit = 1000000)
{
    uint64_t a = 3, b = 7, r = 0, s = 1, q = limit;
    while (q > 2)
    {   uint64_t p = (a * q - 1) / b;
        if (p * s > r * q)
            s = q, r = p;
        q--;
    }
    return twostring(r);
}

/*
#72: Counting fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d
and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions
for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2,
4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 21 elements in this set.

How many elements would be contained in the set of
reduced proper fractions for d <= 1,000,000?

Antwoord: 303,963,552,391
*/

static string problem72(uint64_t L = 1000000)
{   uint64_t *phi = new uint64_t[1000001];
    for (uint64_t i = 0; i <= L; i++)
        phi[i] = i;
    for (uint64_t n = 2; n <= L; n++)
    {   if (phi[n] == n)
        {   for (uint64_t k = n; k <= L; k += n)
                phi[k] -= phi[k] / n;
        }
    }
    uint64_t xsum = 0;
    for (uint64_t i = 0; i <= L; i++)
        xsum += phi[i];
    delete[] phi;
    return twostring<uint64_t>(xsum - 1);
}

/*
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
*/

static string problem73()
{   uint64_t L = 12000, n1 = 1, d1 = 3, n2 = 1, d2 = 2;
    uint64_t *n = new uint64_t[L + 1];
    for (uint64_t i = 0; i <= L; i++) n[i] = 0; // zero out n
    for (uint64_t d = 1; d <= L; d++)
    {   n[d] += ceildiv(n2 * d, d2) - ceildiv(n1 * d, d1) - 1;
        for (uint64_t i = 2 * d; i <= L; i += d)
            n[i] = n[i] - n[d];
    }
    uint64_t xsum = 0;
    for (uint64_t i = 0; i <= L; i++)
        xsum += n[i];
    delete[] n;
    return twostring<uint64_t>(xsum);
}

/*
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
*/

#include <set>

static uint64_t dfccnt(uint64_t *cache, uint64_t n)
{   set<uint64_t> previous;
    uint64_t count = 0;
    while (true)
    {   if (n < 1000000 && cache[n] > 0) return count + cache[n];
        count++;
        if (previous.count(n)) return count;
        previous.insert(n);
        uint64_t xsum = 0;
        while (n > 0) xsum += factorial<uint64_t>(n % 10), n = n / 10;
        n = xsum;
    }
}

static string problem74()
{   uint64_t *cache = new uint64_t[1000000];
    for (uint32_t i = 0; i < 1000000; i++) cache[i] = 0;
    for (uint64_t n = 3; n < 1000000; n++)
        cache[n] = dfccnt(cache, n);
    uint32_t xsum = 0;
    for (uint32_t i = 0; i < 1000000; i++)
        xsum += cache[i] == 61 ? 1 : 0;
    delete[] cache;
    return twostring<uint32_t>(xsum);
}

/*
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
*/

static string problem75()
{
    uint64_t L = 1500001;
    set<uint64_t> maybe;
    set<uint64_t> nope;
    uint64_t fsqrt = floorsqrt<uint64_t>(L/2);
    for (uint64_t m = 2; m < fsqrt; m++)
    {   for (int64_t n = m - 1; n > 0; n -= 2)
        {   if (gcd<uint64_t>(m, n) == 1)
            {   uint64_t s = 2 * (m * m + m * n);
                for (uint64_t k = 1; k <= L / s; k++)
                {   if (maybe.count(k * s))
                        nope.insert(k * s);
                    else
                        maybe.insert(k * s);
                }
            }
        }
    }
    return twostring<uint32_t>(maybe.size() - nope.size());
}

/*
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
*/

static string problem76()
{   uint32_t target = 100, coins[100];
    for (uint32_t i = 1; i < 100; i++) coins[i - 1] = i;
    return twostring<uint32_t>(ways32(target, coins, coins + 99));
}

/*
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
*/

static string problem77()
{   uint32_t lprimes[100], end = 0, n;
    Sieve sieve(100);
    while (sieve.hasNext())
        lprimes[end++] = sieve.next();
    for (n = 10; n < 100; n++)
        if (ways32(n, lprimes, lprimes + end) > 5000) break;
    return twostring<uint32_t>(n);
}

/*
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
*/

/*
https://euler.stephan-brumme.com/78/
*/

static string problem78()
{   uint32_t limit = 100000, nparts = 0; // the solution is < 100000, program ab
    uint64_t *partitions = new uint64_t[limit];
    partitions[nparts++] = 1;
    const int64_t modulo = 1000000;
    for (uint32_t n = 1; n <= limit; n++)
    {   int64_t sum = 0;
        for (uint32_t i = 0; ; i++) // abort inside loop
        {   int32_t alternate = 1 + (i / 2); // generate the digit 1,1,2,2,3,3,...
            if (i % 2 == 1) alternate = -alternate;
            uint32_t offset = alternate * (3 * alternate - 1) / 2;
            if (n < offset) break;
            if (i % 4 < 2)
                sum += partitions[n - offset]; // i % 4 = { 0, 1 }
            else
                sum -= partitions[n - offset]; // i % 4 = { 2, 3 }
            sum %= modulo;
        }
        if (sum < 0) sum += modulo;
        if (sum == 0) break;
        partitions[nparts++] = sum;
    }
    delete[] partitions;
    return twostring<uint32_t>(nparts);
}

/*
#79: Passcode derivation

A common security method used for online banking is to ask the user for
three random characters from a passcode. For example, if the passcode
was 531278, they may ask for the 2nd, 3rd, and 5th characters; the
expected reply would be: 317.

The text file, keylog.txt, contains fifty successful login attempts.

Given that the three characters are always asked for in order, analyse the
file so as to determine the shortest possible secret passcode of unknown length.

Antwoord: 73,162,890
*/

/*
129, 160, 162, 168, 180, 289, 290, 316, 318, 319, 362, 368, 380, 389, 620, 629, 680,
689, 690, 710, 716, 718, 719, 720, 728, 729, 731, 736, 760, 762, 769, 790, 890

73162890
*/

static uint16_t attempts[] = { 319, 680, 180, 690, 129, 620, 762, 689, 762, 318, 368, 710,
    720, 710, 629, 168, 160, 689, 716, 731, 736, 729, 316, 729, 729, 710, 769, 290, 719,
    680, 318, 389, 162, 289, 162, 718, 729, 319, 790, 680, 680, 362, 319, 760, 316, 729,
    380, 319, 728, 716};

bool login79(uint8_t *passcode, uint16_t attempt)
{   uint8_t arr[3];
    arr[0] = linSearch(passcode, passcode + 8, attempt / 100);
    arr[1] = linSearch(passcode, passcode + 8, (attempt / 10) % 10);
    arr[2] = linSearch(passcode, passcode + 8, attempt % 10);
    return arr[1] > arr[0] && arr[2] > arr[1];  // is arr in ascending order?
}

bool test79(uint8_t *perm, uint16_t *attempts)
{   for (uint8_t i = 0; i < 50; i++)
        if (login79(perm, attempts[i]) == false) return false;
    return true;
}

static string problem79()
{   set<uint8_t> digs;
    uint8_t i;
    for (i = 0; i < 50; i++)
    {   uint16_t attempt = attempts[i];
        while (attempt)
        {   digs.insert(attempt % 10);
            attempt = attempt / 10;
        }
    }
    uint8_t size = digs.size(), tmp = 0;
    i = 0;
    uint8_t *pool = new uint8_t[size];
    for (set<uint8_t>::iterator it = digs.begin(); it != digs.end(); it++)
        pool[i++] = *it;
    uint8_t *c = new uint8_t[size];
    for (i = 0; i < size; i++) c[i] = 0;
    for (i = 0; i < size; )
    {   if (c[i] < i)
        {   if (i % 2 == 0) tmp = pool[0], pool[0] = pool[i], pool[i] = tmp;
            else tmp = pool[c[i]], pool[c[i]] = pool[i], pool[i] = tmp;
            c[i]++, i = 0;
            if (test79(pool, attempts)) break;
        }
        else c[i++] = 0;
    }
    delete[] c;
    char ret[9];
    for (uint8_t i = 0; i < 8; i++)
        ret[i] = pool[i] + '0';
    ret[8] = 0;
    delete[] pool;
    return string(ret);
}

/*
#80: Square root digital expansion

It is well known that if the square root of a natural number is not an
integer, then it is irrational. The decimal expansion of such square
roots is infinite without any repeating pattern at all.

The square root of two is 1.41421356237309504880..., and the
digital sum of the first one hundred decimal digits is 475.

For the first one hundred natural numbers, find the total of the digital sums
of the first one hundred decimal digits for all the irrational square roots.

Antwoord: 40,886
*/

/*
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
*/

#if 0
static uint16_t squaredigits(uint8_t square, uint8_t digits = 100)
{   LongNumber25 buf, tmp1, tmp2;
    for (uint8_t digit = 0; digit < digits; digit++)
    {   buf.shiftup();
        for (uint8_t n = 0; n < 10; n++)
        {   buf.setdig(0, 9 - n);
            tmp1.set(buf);
            tmp1.mul2(buf);
            tmp2.set(square);
            tmp2.shiftup(digit * 2);
            if (tmp1.lt(tmp2)) break;
        }
    }
    uint16_t xsum = 0;
    for (uint8_t *it = buf.begin(); it != buf.end(); it++)
        xsum += *it;
    return xsum;
}

static string problem80b()
{   uint32_t xsum = 0;
    for (uint8_t n = 2; n < 100; n++)
    {   if (issquare3(n) == false)
        {   uint16_t tmp = squaredigits(n, 100);
            xsum += tmp;
        }
    }
    return twostring<uint32_t>(xsum);
}
#endif

/*
https://euler.stephan-brumme.com/80/
*/

#include <queue>
#include <vector>

struct BigNum : public std::vector<unsigned int>
{
  static const uint32_t MaxDigit = 1000000000;
  BigNum(uint64_t x = 0)
  {
    do
    {
      push_back(x % MaxDigit);
      x /= MaxDigit;
    } while (x > 0);
  }

  void operator+=(unsigned int other)
  {
    unsigned int carry = other;
    for (size_t i = 0; i < size(); i++)
    {
      carry += operator[](i);
      if (carry == 0)
            return;

      if (carry < MaxDigit)
      {
        operator[](i) = carry;
        carry         = 0;
      }
      else
      {
        operator[](i) = carry % MaxDigit;
        carry         = carry / MaxDigit;
      }
    }

    while (carry > 0)
    {
      push_back(carry % MaxDigit);
      carry /= MaxDigit;
    }
  }

  void operator+=(const BigNum& other)
  {
    if (size() < other.size())
      resize(other.size(), 0);

    unsigned int carry = 0;
    for (size_t i = 0; i < size(); i++)
    {
      carry += operator[](i);
      if (i < other.size())
        carry += other[i];
      else
        if (carry == 0)
          return;

      if (carry < MaxDigit)
      {
        // no overflow
        operator[](i) = carry;
        carry     = 0;
      }
      else
      {
        // yes, we have an overflow
        operator[](i) = carry - MaxDigit;
        carry = 1;
      }
    }

    if (carry > 0)
      push_back(carry);
  }

  void operator-=(const BigNum& other)
  {
    int borrow = 0;
    for (size_t i = 0; i < size(); i++)
    {
      int diff = (int)operator[](i) - borrow;
      if (i < other.size())
        diff -= other[i];
      else
        if (borrow == 0)
          break;

      if (diff < 0)
      {
        borrow = 1;
        diff += MaxDigit;
      }
      else
        borrow = 0;

      operator[](i) = diff;
    }

    // no high zeros
    while (size() > 1 && back() == 0)
      pop_back();
  }

  void operator*=(unsigned int factor)
  {
    // nulled
    if (factor == 0)
    {
      clear();
      push_back(0);
      return;
    }
    // unchanged
    if (factor == 1)
      return;

    // append an empty block
    if (factor == MaxDigit)
    {
      if (size() > 1 || operator[](0) > 0)
        insert(begin(), 0);
      return;
    }

    // multiply all blocks with the factor
    unsigned long long carry = 0;
    for (auto& i : *this)
    {
      carry += i * (unsigned long long)factor;
      i      = carry % MaxDigit;
      carry /= MaxDigit;
    }
    // store remaining carry in new digits
    while (carry > 0)
    {
      push_back(carry % MaxDigit);
      carry /= MaxDigit;
    }
  }

  // multiply two big numbers
  BigNum operator*(const BigNum& other) const
  {
    if (size() < other.size())
      return other * *this;

    // multiply single digits of "other" with the current object
    BigNum result = 0;
    result.reserve(size() + other.size());
    for (int i = (int)other.size() - 1; i >= 0; i--)
    {
      BigNum temp = *this;
      temp   *= other[i];

      result *= MaxDigit;
      result += temp;
    }

    return result;
  }

  bool operator<(const BigNum& other) const
  {
    // different number of digits/buckets ?
    if (size() < other.size())
      return true;
    if (size() > other.size())
      return false;
    // compare all digits/buckets, beginning with the most significant
    for (int i = (int)size() - 1; i >= 0; i--)
    {
      if (operator[](i) < other[i])
        return true;
      if (operator[](i) > other[i])
        return false;
    }
    return false;
  }

  // convert to string, MaxDigit must be power of 10
  std::string toString() const
  {
    std::string result;
    for (auto x : *this)
    {
      // process a bucket
      for (unsigned int shift = 1; shift < MaxDigit; shift *= 10)
      {
        auto digit = (x / shift) % 10;
        result.insert(0, 1, (char)digit + '0');
      }
    }

    // remove leading zeros
    while (result.size() > 1 && result.front() == '0')
      result.erase(0, 1);

    return result;
  }
};

BigNum jarvis(unsigned int x, const BigNum& precision)
{
  static const BigNum Fortyfive = 45;

  BigNum a = x * 5;
  BigNum b = 5;

  // avoid re-allocations when growing (plus a few bytes when exceeding target)
  a.reserve(precision.size());
  b.reserve(precision.size());

  // until we have enough digits
  while (b < precision)
  {
    if (!(a < b)) // same as a >= b but currently there is no operator >= in my BigNum class
    {
      a -=   b;
      b +=  10;
    }
    else
    {
      a *= 100;
      b *=  10;
      b -= Fortyfive;
    }
  }

  return b;
}

unsigned int countDigits(const BigNum& x, unsigned int numDigits)
{
  unsigned int sum = 0;
  for (auto i : x.toString().substr(0, numDigits))
    sum += i - '0';
  return sum;
}

static string problem80()
{
    unsigned int maxNumber = 100;
    unsigned int digits    = 100;
    const unsigned int ExtraDigits = 15;
    BigNum precision = 10;
    for (unsigned int i = 1; i < digits + ExtraDigits; i++)
        precision *= 10;

    vector<BigNum> roots(maxNumber + 1, 0);
    unsigned int sum = 0;
    for (unsigned int i = 1; i <= maxNumber; i++)
    {
        uint32_t intSqrt = 1;
        while (intSqrt * intSqrt < i)
            intSqrt++;
        if (intSqrt * intSqrt == i)
        {
            roots[i] = precision * intSqrt;
            continue;
        }

        auto factor = intSqrt - 1;
        while (i % factor != 0)
            factor--;

    if (factor > 1)
    {
      auto& current = roots[i] = roots[i / factor] * roots[factor];

      if (current.size() > roots[i - 1].size())
        current.erase(current.begin(), current.begin() + (current.size() - roots[i - 1].size()));
      while (current < roots[i - 1])
        current *= 10;
    }
    else
    {
      roots[i] = jarvis(i, precision);
    }
    sum += countDigits(roots[i], digits);
  }

  return twostring<uint32_t>(sum);
}

/*
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
*/

static string problem81()
{   ifstream ifs;
    ifs.open("euler81.txt");
    uint16_t x = 0, n = 0;
    char c;
    uint32_t arr[80*80];
    while ((ifs.get(c)))
    {   if (isdigit(c))
            x = x * 10 + (c - '0');
        else if (x > 0)
            arr[n++] = x, x = 0;
    }
    ifs.close();
    uint16_t root = floorsqrt<uint32_t>(n);
    for (uint16_t i = 0; i < root - 1; i++)
        arr[i + 1] += arr[i];
    for (uint16_t i = 0; i < root - 1; i++)
        arr[(i + 1) * root] += arr[i * root];
    for (uint16_t i = 0; i < root - 1; i++)
        for (uint16_t j = 0; j < root - 1; j++)
            arr[(i + 1) * root + (j + 1)] += std::min(arr[(i + 1)*root+j], arr[i*root+(j + 1)]);
    return twostring<uint32_t>(arr[n - 1]);
}

/*
#82: Path sum: three ways

NOTE: This problem is a more challenging version of Problem 81.

Antwoord: 260,324
*/

static string problem82()
{
    ifstream ifs;
    ifs.open("euler82.txt");
    uint16_t x = 0, n = 0;
    char c;
    uint32_t arr[80*80];
    while ((ifs.get(c)))
    {   if (isdigit(c))
            x = x * 10 + (c - '0');
        else if (x > 0)
            arr[n++] = x, x = 0;
    }
    ifs.close();
    uint32_t grid[80][80];
    uint32_t sol[80];
    for (uint16_t i = 0; i < 80*80; i++)
        grid[i / 80][i % 80] = arr[i];
    for (uint16_t i = 0; i < 80; i++)
        sol[i] = grid[i][79];
    for (uint16_t i = 79; i > 0; i--)
    {
        sol[0] += grid[0][i - 1];
        for (uint16_t j = 1; j < 80; j++)
            sol[j] = std::min(sol[j - 1] + grid[j][i - 1], sol[j] + grid[j][i - 1]);
        for (uint16_t j = 79; j > 0; j--)
            sol[j - 1] = std::min(sol[j - 1], sol[j] + grid[j - 1][i - 1]);
    }
    uint32_t xmin = 0xffffffff;
    for (uint8_t i = 0; i < 80; i++)
        xmin = std::min(xmin, sol[i]);
    return twostring<uint32_t>(xmin);
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

/*
https://euler.stephan-brumme.com/83/
*/

typedef std::vector<vector<uint32_t>> Matrix;

struct Cell
{   uint32_t x, y;
    uint64_t weight;
    Cell(uint32_t x_, uint32_t y_, uint64_t weight_) : x(x_), y(y_), weight(weight_) {}
    bool operator<(const Cell& cell) const { return weight > cell.weight; }
};

static uint64_t search(const Matrix& matrix)
{   const auto size = matrix.size();
    vector<std::vector<bool>> processed(matrix.size());
    for (auto& row : processed)
        row.resize(matrix.size(), false);
    std::priority_queue<Cell> next;
    next.push(Cell(0, 0, matrix[0][0]));
    while (!next.empty())
    {   Cell cell = next.top();
        next.pop();
        if (processed[cell.y][cell.x])
            continue;
        processed[cell.y][cell.x] = true;
        if (cell.x == size - 1 && cell.y == size - 1)
            return cell.weight;
        if (cell.x + 1 < size)
            next.push(Cell(cell.x + 1, cell.y, cell.weight + matrix[cell.y][cell.x + 1]));
        if (cell.y + 1 < size)
            next.push(Cell(cell.x, cell.y + 1, cell.weight + matrix[cell.y + 1][cell.x]));
        if (cell.y > 0)
            next.push(Cell(cell.x, cell.y - 1, cell.weight + matrix[cell.y - 1][cell.x]));
        if (cell.x > 0)
            next.push(Cell(cell.x - 1, cell.y, cell.weight + matrix[cell.y][cell.x - 1]));
    }
    return -1; // failed
}

static string problem83()
{   ifstream ifs;
    ifs.open("euler83.txt");
    const uint16_t size = 80;
    uint16_t x = 0, n = 0;
    char c;
    uint32_t arr[80*80];
    while ((ifs.get(c)))
    {   if (isdigit(c))
            x = x * 10 + (c - '0');
        else if (x > 0)
            arr[n++] = x, x = 0;
    }
    ifs.close();
    Matrix matrix(size);
    uint16_t i = 0;
    for (auto &row : matrix)
    {   row.resize(size);
        for (auto &cell : row)
            cell = arr[i++];
    }
    return twostring<uint32_t>(search(matrix));
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

class Mersenne : public Generator<uint32_t>
{
private:
    uint32_t state[624];
    uint32_t f = 1812433253;
    uint32_t m = 397;
    uint32_t u = 11;
    uint32_t s = 7;
    uint32_t b = 0x9D2C5680;
    uint32_t t = 15;
    uint32_t c = 0xEFC60000;
    uint32_t l = 18;
    uint32_t index = 624;
    uint32_t lower_mask = (1UL<<31)-1;
    uint32_t upper_mask = 1<<31;
public:
    Mersenne(uint32_t seed)
    {
        state[0] = seed;
        for (uint16_t i = 1; i < 624; i++)
            state[i] = f * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
    }
    void twist()
    {   for (uint16_t i = 0; i < 624; i++)
        {   uint32_t tmp = (state[i] & upper_mask) + (state[(i + 1) % 624] & lower_mask);
            uint32_t tmp_shift = tmp >> 1;
            if (tmp % 2 != 0)
                tmp_shift = tmp_shift ^ 0x9908b0df;
            state[i] = state[(i + m) % 624] ^ tmp_shift;
        }
        index = 0;
    }
    bool hasNext() { return true; }
    uint32_t next()
    {   if (index >= 624) twist();
        uint32_t y = state[index];
        y = y ^ (y >> u);
        y = y ^ ((y << s) & b);
        y = y ^ ((y << t) & c);
        y = y ^ (y >> l);
        index++;
        return y;
    }
};

class CC : public Generator<uint32_t>
{
private:
    uint32_t _i = 0;
public:
    static const uint32_t GO = 0, JAIL = 1;
    bool hasNext() { return true; }
    uint32_t next()
    {
        uint32_t ret = _i;
        _i = (_i + 1) % 16;
        return ret;
    }
};

class CH : public Generator<uint32_t>
{
private:
    uint32_t _i = 0;
public:
    static const uint32_t GO = 0, JAIL = 1, C1 = 2, E3 = 3, H2 = 4, R1 = 5, NR1 = 6;
    static const uint32_t NR2 = 7, U = 8, BACK3 = 9;
    bool hasNext() { return true; }
    uint32_t next()
    {
        uint32_t ret = _i;
        _i = (_i + 1) % 16;
        return ret;
    }
};

class Monopoly
{
private:
    uint32_t _pos = 0;
    uint32_t _doubles = 0;
    uint32_t hits[40];
    Mersenne _rng;
    CC _ccgen;
    CH _chgen;
public:
    static const uint32_t GO = 0;
    static const uint32_t A1 = 1;
    static const uint32_t CC1 = 2;
    static const uint32_t R1 = 5;
    static const uint32_t CH1 = 7;
    static const uint32_t JAIL = 10;
    static const uint32_t C1 = 11;
    static const uint32_t U1 = 12;
    static const uint32_t R2 = 15;
    static const uint32_t D1 = 16;
    static const uint32_t CC2 = 17;
    static const uint32_t CH2 = 22;
    static const uint32_t E2 = 23;
    static const uint32_t E3 = 24;
    static const uint32_t R3 = 25;
    static const uint32_t U2 = 28;
    static const uint32_t G2J = 30;
    static const uint32_t G1 = 31;
    static const uint32_t G2 = 32;
    static const uint32_t CC3 = 33;
    static const uint32_t G3 = 34;
    static const uint32_t R4 = 35;
    static const uint32_t CH3 = 36;
    static const uint32_t H1 = 37;
    static const uint32_t T2 = 38;
    static const uint32_t H2 = 39;
    Monopoly() : _rng(1131464071)
    {
        for (uint8_t i = 0; i < 40; i++) hits[i] = 0;
    }
    void roll();
    uint32_t *begin() { return hits; }
    uint32_t *end() { return hits + 40; }
};

void Monopoly::roll()
{
    uint32_t dice1 = _rng.next() % 4 + 1;
    uint32_t dice2 = _rng.next() % 4 + 1;
    uint32_t dice = dice1 + dice2;
    _pos = (_pos + dice) % 40;
    _doubles = dice1 == dice2 ? _doubles + 1 : 0;
    if (_doubles == 3)
    {   _doubles = 0;
        _pos = JAIL;
    }
    switch (_pos)
    {
    case CH1:
    case CH2:
    case CH3:
        switch (_chgen.next())
        {
        case CH::GO:
            _pos = GO;
            break;
        case CH::JAIL:
            _pos = JAIL;
            break;
        case CH::C1:
            _pos = C1;
            break;
        case CH::E3:
            _pos = E3;
            break;
        case CH::H2:
            _pos = H2;
            break;
        case CH::R1:
            _pos = R1;
            break;
        case CH::NR1:
        case CH::NR2:
            if (_pos == CH1) _pos = R2;
            if (_pos == CH2) _pos = R3;
            if (_pos == CH3) _pos = R1;
            break;
        case CH::U:
            if (_pos < U1 || _pos > U2) _pos = U1;
            if (_pos < U2 || _pos > U1) _pos = U2;
            break;
        case CH::BACK3:
            _pos -= 3;
            break;
        }
        break;
    case CC1:
    case CC2:
    case CC3:
        switch (_ccgen.next())
        {
        case CC::GO:
            _pos = GO;
            break;
        case CC::JAIL:
            _pos = JAIL;
            break;
        }
        break;
    case G2J:
        _pos = JAIL;
        break;
    }
    hits[_pos] += 1;
}

static string problem84()
{   Monopoly game;
    for (uint32_t i = 0; i < 999999; i++)
        game.roll();
    uint32_t sorted[40];
    for (uint32_t *it = game.begin(), i = 0; it != game.end(); it++)
        sorted[i++] = *it;
    bubbleSort(sorted, sorted + 40);
    uint32_t first = linSearch(game.begin(), game.end(), sorted[39]) - 1;
    uint32_t second = linSearch(game.begin(), game.end(), sorted[38]) - 1;
    uint32_t third = linSearch(game.begin(), game.end(), sorted[37]) - 1;
    uint32_t ret = first * 10000 + second * 100 + third;
    return twostring<uint32_t>(ret);
}

/*
#85: Counting rectangles

By counting carefully it can be seen that a rectangular
grid measuring 3 by 2 contains eighteen rectangles:

Although there exists no rectangular grid that contains exactly two
million rectangles, find the area of the grid with the nearest solution.

Antwoord: 2,772
*/

static string problem85()
{   int32_t best_x = 0xffff, best_y = 0xffff, best_rects = 0xffff;
    for (int32_t x = 1; x <= 2000; x++)
    {   for (int32_t y = 1; y <= 2000; y++)
        {   int32_t diff = std::abs((x*x+x)*(y*y+y)/4 - 2000000);
            if (diff < best_rects)
                best_x = x, best_y = y, best_rects = diff;
        }
    }
    return twostring<uint32_t>(best_x * best_y);
}

/*
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
*/

static string problem86()
{   uint32_t L = 1000000, c = 0, a = 2;
    while (c < L)
    {   a++;
        for (uint32_t bc = 3; bc < 2 * a; bc++)
            if (bc * a % 12 == 0 && issquare3(bc * bc + a*a))
                c += std::min(bc, a + 1) - (bc + 1) / 2;
    }
    return twostring<uint32_t>(a);
}

/*
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
*/

static string problem87()
{   set<uint32_t> P;
    Sieve sa(7072);
    while (sa.hasNext())
    {   uint32_t a = sa.next();
        Sieve sb(369);
        while (sb.hasNext())
        {   uint32_t b = sb.next();
            Sieve sc(85);
            while (sc.hasNext())
            {   uint32_t c = sc.next();
                uint32_t q = a*a + b*b*b + c*c*c*c;
                if (q >= 50000000) break;
                P.insert(q);
            }
        }
    }
    return twostring<uint32_t>(P.size());
}

/*
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
*/

static void prodsum(uint64_t p, uint64_t s, uint64_t c,
    uint64_t start, uint64_t kmax, uint64_t *n)
{
    uint64_t k = p - s + c;
    if (k < kmax)
    {   if (p < n[k]) n[k] = p;
        for (uint64_t i = start; i <= kmax / p * 2; i++)
            prodsum(p * i, s + i, c + 1, i, kmax, n);
    }
}

static string problem88()
{   uint64_t kmax = 12001;
    uint64_t n[12001];
    for (uint64_t i = 0; i < 12001; i++)
        n[i] = 2 * kmax;
    prodsum(1, 1, 1, 2, kmax, n);
    n[0] = 0;
    n[1] = 0;
    bubbleSort(n, n + 12001);
    uint64_t xsum = 0, prev = 0;
    for (uint64_t i = 0; i < 12001; i++)
    {   if (n[i] != prev) xsum += n[i];
        prev = n[i];
    }
    return twostring<uint64_t>(xsum);
}

/*
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
*/

#include <cstring>

class Text
{
private:
    char *_buf;
    uint32_t _i = 0;
public:
    Text(uint32_t size) { _buf = new char[size]; }
    ~Text() { delete[] _buf; }
    void push(char c) { _buf[_i++] = c; _buf[_i] = 0; }
    void replace(const char *a, const char *b)
    {   size_t lena = strlen(a);
        size_t lenb = strlen(b);
        while (true)
        {   char *beg = strstr(_buf, a);
            if (beg == 0) break;
            for (size_t i = 0; i < lenb; i++)
                *beg++ = b[i];
            while (true)
            {   beg[0] = beg[lena - lenb];
                if (beg[lena - lenb] == 0) break;
                beg++;
            }
        }
    }
    uint32_t length() { return strlen(_buf); }
};

static string problem89()
{   ifstream ifs;
    ifs.open("euler89.txt");
    char c;
    Text t(9999);
    while (ifs.get(c))
        t.push(c);
    ifs.close();
    uint32_t in = t.length();
    t.replace("DCCCC", "  ");
    t.replace("LXXXX", "  ");
    t.replace("VIIII", "  ");
    t.replace("CCCC", "  ");
    t.replace("XXXX", "  ");
    t.replace("IIII", "  ");
    uint32_t out = t.length();
    return twostring<uint32_t>(in - out);
}

/*
#90: Cube digit pairs

Antwoord: 1,217
*/

static bool valid(uint8_t *c1begin, uint8_t *c2begin, uint8_t *sqbegin, uint8_t *sqend)
{   for (uint8_t *it = sqbegin; it != sqend; it++)
    {   bool x_in_c1 = linSearch(c1begin, c1begin + 6, (*it & 0xf0) >> 4) > 0;
        bool y_in_c2 = linSearch(c2begin, c2begin + 6, *it & 0xf) > 0;
        bool x_in_c2 = linSearch(c2begin, c2begin + 6, (*it & 0xf0) >> 4) > 0;
        bool y_in_c1 = linSearch(c1begin, c1begin + 6, *it & 0xf) > 0;
        if ((x_in_c1 && y_in_c2) || (x_in_c2 && y_in_c1))
            continue;
        return false;
    }
    return true;
}

// TODO: combinations generator
static string problem90()
{   uint8_t squares[] = {0x01, 0x04, 0x06, 0x16, 0x25, 0x36, 0x46, 0x81};
    uint8_t cube[] = {0,1,2,3,4,5, 0,1,2,3,4,6, 0,1,2,3,4,7, 0,1,2,3,4,8, 0,1,2,3,4,6,
                      0,1,2,3,5,6, 0,1,2,3,5,7, 0,1,2,3,5,8, 0,1,2,3,5,6, 0,1,2,3,6,7,
                      0,1,2,3,6,8, 0,1,2,3,6,6, 0,1,2,3,7,8, 0,1,2,3,7,6, 0,1,2,3,8,6,
                      0,1,2,4,5,6, 0,1,2,4,5,7, 0,1,2,4,5,8, 0,1,2,4,5,6, 0,1,2,4,6,7,
                      0,1,2,4,6,8, 0,1,2,4,6,6, 0,1,2,4,7,8, 0,1,2,4,7,6, 0,1,2,4,8,6,
                      0,1,2,5,6,7, 0,1,2,5,6,8, 0,1,2,5,6,6, 0,1,2,5,7,8, 0,1,2,5,7,6,
                      0,1,2,5,8,6, 0,1,2,6,7,8, 0,1,2,6,7,6, 0,1,2,6,8,6, 0,1,2,7,8,6,
                      0,1,3,4,5,6, 0,1,3,4,5,7, 0,1,3,4,5,8, 0,1,3,4,5,6, 0,1,3,4,6,7,
                      0,1,3,4,6,8, 0,1,3,4,6,6, 0,1,3,4,7,8, 0,1,3,4,7,6, 0,1,3,4,8,6,
                      0,1,3,5,6,7, 0,1,3,5,6,8, 0,1,3,5,6,6, 0,1,3,5,7,8, 0,1,3,5,7,6,
                      0,1,3,5,8,6, 0,1,3,6,7,8, 0,1,3,6,7,6, 0,1,3,6,8,6, 0,1,3,7,8,6,
                      0,1,4,5,6,7, 0,1,4,5,6,8, 0,1,4,5,6,6, 0,1,4,5,7,8, 0,1,4,5,7,6,
                      0,1,4,5,8,6, 0,1,4,6,7,8, 0,1,4,6,7,6, 0,1,4,6,8,6, 0,1,4,7,8,6,
                      0,1,5,6,7,8, 0,1,5,6,7,6, 0,1,5,6,8,6, 0,1,5,7,8,6, 0,1,6,7,8,6,
                      0,2,3,4,5,6, 0,2,3,4,5,7, 0,2,3,4,5,8, 0,2,3,4,5,6, 0,2,3,4,6,7,
                      0,2,3,4,6,8, 0,2,3,4,6,6, 0,2,3,4,7,8, 0,2,3,4,7,6, 0,2,3,4,8,6,
                      0,2,3,5,6,7, 0,2,3,5,6,8, 0,2,3,5,6,6, 0,2,3,5,7,8, 0,2,3,5,7,6,
                      0,2,3,5,8,6, 0,2,3,6,7,8, 0,2,3,6,7,6, 0,2,3,6,8,6, 0,2,3,7,8,6,
                      0,2,4,5,6,7, 0,2,4,5,6,8, 0,2,4,5,6,6, 0,2,4,5,7,8, 0,2,4,5,7,6,
                      0,2,4,5,8,6, 0,2,4,6,7,8, 0,2,4,6,7,6, 0,2,4,6,8,6, 0,2,4,7,8,6,
                      0,2,5,6,7,8, 0,2,5,6,7,6, 0,2,5,6,8,6, 0,2,5,7,8,6, 0,2,6,7,8,6,
                      0,3,4,5,6,7, 0,3,4,5,6,8, 0,3,4,5,6,6, 0,3,4,5,7,8, 0,3,4,5,7,6,
                      0,3,4,5,8,6, 0,3,4,6,7,8, 0,3,4,6,7,6, 0,3,4,6,8,6, 0,3,4,7,8,6,
                      0,3,5,6,7,8, 0,3,5,6,7,6, 0,3,5,6,8,6, 0,3,5,7,8,6, 0,3,6,7,8,6,
                      0,4,5,6,7,8, 0,4,5,6,7,6, 0,4,5,6,8,6, 0,4,5,7,8,6, 0,4,6,7,8,6,
                      0,5,6,7,8,6, 1,2,3,4,5,6, 1,2,3,4,5,7, 1,2,3,4,5,8, 1,2,3,4,5,6,
                      1,2,3,4,6,7, 1,2,3,4,6,8, 1,2,3,4,6,6, 1,2,3,4,7,8, 1,2,3,4,7,6,
                      1,2,3,4,8,6, 1,2,3,5,6,7, 1,2,3,5,6,8, 1,2,3,5,6,6, 1,2,3,5,7,8,
                      1,2,3,5,7,6, 1,2,3,5,8,6, 1,2,3,6,7,8, 1,2,3,6,7,6, 1,2,3,6,8,6,
                      1,2,3,7,8,6, 1,2,4,5,6,7, 1,2,4,5,6,8, 1,2,4,5,6,6, 1,2,4,5,7,8,
                      1,2,4,5,7,6, 1,2,4,5,8,6, 1,2,4,6,7,8, 1,2,4,6,7,6, 1,2,4,6,8,6,
                      1,2,4,7,8,6, 1,2,5,6,7,8, 1,2,5,6,7,6, 1,2,5,6,8,6, 1,2,5,7,8,6,
                      1,2,6,7,8,6, 1,3,4,5,6,7, 1,3,4,5,6,8, 1,3,4,5,6,6, 1,3,4,5,7,8,
                      1,3,4,5,7,6, 1,3,4,5,8,6, 1,3,4,6,7,8, 1,3,4,6,7,6, 1,3,4,6,8,6,
                      1,3,4,7,8,6, 1,3,5,6,7,8, 1,3,5,6,7,6, 1,3,5,6,8,6, 1,3,5,7,8,6,
                      1,3,6,7,8,6, 1,4,5,6,7,8, 1,4,5,6,7,6, 1,4,5,6,8,6, 1,4,5,7,8,6,
                      1,4,6,7,8,6, 1,5,6,7,8,6, 2,3,4,5,6,7, 2,3,4,5,6,8, 2,3,4,5,6,6,
                      2,3,4,5,7,8, 2,3,4,5,7,6, 2,3,4,5,8,6, 2,3,4,6,7,8, 2,3,4,6,7,6,
                      2,3,4,6,8,6, 2,3,4,7,8,6, 2,3,5,6,7,8, 2,3,5,6,7,6, 2,3,5,6,8,6,
                      2,3,5,7,8,6, 2,3,6,7,8,6, 2,4,5,6,7,8, 2,4,5,6,7,6, 2,4,5,6,8,6,
                      2,4,5,7,8,6, 2,4,6,7,8,6, 2,5,6,7,8,6, 3,4,5,6,7,8, 3,4,5,6,7,6,
                      3,4,5,6,8,6, 3,4,5,7,8,6, 3,4,6,7,8,6, 3,5,6,7,8,6, 4,5,6,7,8,6};
    uint32_t xsum = 0;
    for (uint32_t i = 0; i < 210; i++)
        for (uint32_t j = 0; j < i; j++)
            xsum += valid(cube + i * 6, cube + j * 6, squares, squares + 8) ? 1 : 0;
    return twostring<uint32_t>(xsum);
}

/*
#91: Right triangles with integer coordinates

Antwoord: 14,234
*/

static string problem91()
{   uint32_t n = 50, t = 0;
    for (uint32_t x = 1; x <= n; x++)
    {   for (uint32_t y = 1; y < n; y++)
        {   uint32_t m = gcd<uint32_t>(x, y);
            t += std::min(x * m / y, m * (n - y) / x);
        }
    }
    return twostring<uint32_t>(t * 2 + n * n * 3);
}

/*
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
*/

static uint64_t sos_digits(uint64_t n)
{   uint64_t s = 0;
    while (n > 0)
        s = s + (n % 10)*(n % 10), n = n / 10;
    return s;
}

static bool unhappy(uint64_t n)
{   while (n > 1 && n != 89 && n != 4)
        n = sos_digits(n);
    return n > 1;
}

static string problem92()
{   uint64_t L = 7;
    uint64_t Lc = 9*9 * L + 1;
    uint64_t *t = new uint64_t[Lc];
    uint64_t *solutions = new uint64_t[Lc];
    for (uint64_t i = 0; i < Lc; i++)
        t[i] = 0, solutions[i] = 0;
    for (uint64_t i = 0; i < 10; i++)
        solutions[i * i] = 1;
    for (uint64_t i = 2; i <= L; i++)
    {   for (uint64_t j = 0; j < Lc; j++)
        {   t[j] = 0;
            for (uint64_t k = 0; k < 10; k++)
                t[j] += k * k <= j ? solutions[j - k*k] : 0;
        }
        for (uint64_t j = 0; j < Lc; j++)
            solutions[j] = t[j];
    }
    uint64_t ret = 0;
    for (uint64_t i = 1; i < Lc; i++)
        ret += unhappy(i) ? solutions[i] : 0;
    delete[] solutions;
    delete[] t;
    return twostring<uint64_t>(ret);
}

/*
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
*/

/*
https://euler.stephan-brumme.com/93/
*/

#include <cmath>

const double Epsilon = 0.00001;

void eval(const std::vector<double>& numbers, std::vector<bool>& used)
{
    if (numbers.size() == 1)
    {
        auto result = numbers.front() + Epsilon;
        if (fmod(result, 1) > 10*Epsilon)
            return;

        int index = int(result + Epsilon);
        // reject negative and very large results
        if (index >= 0 && index < (int)used.size())
            used[index] = true;

        return;
    }
    // step 2
    auto next = numbers;
    for (size_t i = 0; i < numbers.size(); i++)
    for (size_t j = i + 1; j < numbers.size(); j++)
    {
      // fetch two numbers
      double a = numbers[i];
      double b = numbers[j];

      // prepare for next recursive step
      next = numbers;
      next.erase(next.begin() + j); // delete the higher index first
      next.erase(next.begin() + i);

      // steps 3 and 4 (unrolled)
      next.push_back(a + b); // add
      eval(next, used);
      next.back() = a - b;   // subtract (I)
      eval(next, used);
      next.back() = b - a;   // subtract (II)
      eval(next, used);
      next.back() = a * b;   // multiply
      eval(next, used);
      if (b != 0)
      {
        next.back() = a / b; // divide (I)
        eval(next, used);
      }
      if (a != 0)
      {
        next.back() = b / a; // divide (II)
        eval(next, used);
      }
    }
}

unsigned int getSequenceLength(const std::vector<double>& numbers)
{   vector<bool> used(1000, false);
    eval(numbers, used);
    unsigned int result = 0;
    while (used[result + 1])
        result++;
    return result;
}

static string problem93()
{   unsigned int longestSequence = 0;
    unsigned int longestDigits   = 0;

    for (unsigned int a =   1; a <= 6; a++)
        for (unsigned int b = a+1; b <= 7; b++)
            for (unsigned int c = b+1; c <= 8; c++)
                for (unsigned int d = c+1; d <= 9; d++)
                {
                    auto sequenceLength = getSequenceLength({ double(a), double(b),
                        double(c), double(d) });

                    if (longestSequence < sequenceLength)
                    {
                        longestSequence = sequenceLength;
                        longestDigits = a * 1000 + b * 100 + c * 10 + d;
                    }
                }

    return twostring<uint32_t>(longestDigits);
}

/*
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
*/

static string problem94()
{   int64_t side0 = 1, side = 1, s = 0, p = 0, m = 1, L = 1000000000;
    while (p <= L)
    {   int64_t tmp = 4 * side - side0 + 2 * m;
        side0 = side;
        side = tmp;
        m = -m;
        s += p;
        p = 3 * side - m;
    }
    return twostring<uint64_t>(s);
}

/*
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
*/

/*
https://euler.stephan-brumme.com/95/
*/

static string problem95()
{
    uint32_t limit = 1000000;
    vector<uint32_t> primes;
    primes.push_back(2);
    for (uint32_t i = 3; i <= limit; i += 2)
    {
        bool isPrime = true;
        for (auto p : primes)
        {
            if (p*p > i) break;
            if (i % p == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            primes.push_back(i);
    }
    vector<uint32_t> divsum(limit + 1, 0);
    for (uint32_t i = 2; i <= limit; i++)
    {   uint32_t sum = 1, reduce = i;
        for (auto p : primes)
        {   if (p*p > reduce) break;
            uint32_t factor = 1;
            while (reduce % p == 0)
            {   reduce /= p;
                factor *= p;
                factor++;
            }
            sum *= factor;
        }
        if (reduce > 1 && reduce < i) sum *= reduce + 1;
        if (sum > 1) sum -= i;
        divsum[i] = sum;
    }

    uint32_t longestChain = 0, smallestMember = limit;
    for (uint32_t i = 1; i <= limit; i++)
    {
    static vector<uint32_t> chain;
    chain.clear();
    chain.push_back(i);
    while (true)
    {
      uint32_t add = divsum[chain.back()];
      chain.push_back(add);

      if (add == i)
        break;

      if (add < i)
        break;

      if (add > limit)
        break;
      bool isLoop = false;
      for (size_t j = 1; j < chain.size() - 1; j++) // except last element
        if (add == chain[j])
        {
          isLoop = true;
          break;
        }
      if (isLoop)
        break;
    }

    if (chain.back() != i)
      continue;

    if (chain.size() < longestChain)
      continue;

    if (longestChain < chain.size())
    {
      longestChain = chain.size();
      smallestMember = chain.front();
    }
  }
    return twostring<uint32_t>(smallestMember);
}

/*
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
*/

typedef uint32_t Board[9][9];

bool solve(Board &board)
{   for (uint32_t y = 0; y < 9; y++)
    {   for (uint32_t x = 0; x < 9; x++)
        {   if (board[x][y] != 0) continue;
            bool available[9+1] = {false, true, true, true, true, true, true, true, true, true};
            for (uint32_t i = 0; i < 9; i++)
            {   if (board[i][y] != 0) available[board[i][y]] = false;
                if (board[x][i] != 0) available[board[x][i]] = false;
            }
            uint32_t rx = (x / 3) * 3, ry = (y / 3) * 3;
            for (uint32_t i = 0; i < 3; i++)
                for (uint32_t j = 0; j < 3; j++)
                    if (board[i + rx][j + ry] != 0)
                        available[board[i + rx][j + ry]] = false;
            for (uint32_t i = 1; i <= 9; i++)
            {   if (available[i])
                {   board[x][y] = i;
                    if (solve(board))
                        return true;
                }
            }
            board[x][y] = 0;
            return false;
        }
    }
    return true;
}

static string problem96()
{   ifstream ifs;
    ifs.open("euler96.txt");
    uint8_t tmp[81];
    uint32_t board[9][9], i = 0;
    char c;
    uint32_t xsum = 0;
    while (ifs.get(c))
    {   if (isdigit(c))
        {   tmp[i++] = c - '0';
            continue;
        }
        if (i == 0) continue;
        i = 0;
        for (uint8_t j = 0; j < 81; j++)
            board[j / 9][j % 9] = tmp[j];
        solve(board);
        uint32_t subsum = board[0][0] * 100 + board[0][1] * 10 + board[0][2];
        xsum += subsum;
    }
    ifs.close();
    return twostring<uint32_t>(xsum);
}

/*
#97: Large non-Mersenne prime

The first known prime found to exceed one million digits was discovered in
1999, and is a Mersenne prime of the form 26972593-1; it contains exactly
2,098,960 digits. Subsequently other Mersenne primes, of the form 2p-1,
have been found which contain more digits.

However, in 2004 there was found a massive non-Mersenne
prime which contains 2,357,207 digits: 28433x2^7830457+1.

Find the last ten digits of this prime number.

Antwoord: 8,739,992,577
*/

/*
http://code.jasonbhill.com/c/project-euler-97/
*/

static string problem97()
{   uint64_t n = 2;
    for (uint64_t i = 0; i < 7830456; i++)
        n = (2 * n) % 10000000000ULL;
    n = (n * 28433 + 1) % 10000000000ULL;
    return twostring<uint64_t>(n);
}

/*
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
*/

/*
https://blog.dreamshire.com/project-euler-98-solution/
https://euler.stephan-brumme.com/98/
*/

#include <map>
#include <algorithm>

static uint64_t fingerprint(uint64_t x)
{   uint64_t result = 0;
    while (x > 0)
    {   auto digit = x % 10;
        x /= 10;
        result += 1ULL << (4 * digit);
    }
    return result;
}

string readWord(istream &is)
{   string result;
    while (true)
    {   char c = is.get();
        if (!is) break;
        if (c == '"') continue;
        if (c == ',') break;
        result += c;
    }
    return result;
}

uint64_t match(const string &a, const string &b, const vector<unsigned long long> &squares)
{
  uint64_t result = 0;
  // try all combinations
  for (auto i : squares)
    for (auto j : squares)
    {
      // don't match a word with itself
      if (i == j)
        continue;

      // convert squares to strings
      auto replaceA = std::to_string(i);
      auto replaceB = std::to_string(j);


      std::map<char, char> replaceTable;
      bool valid = true;
      for (size_t k = 0; k < replaceA.size(); k++)
      {
        char original = replaceA[k];
        // no replacement rule found ? => abort
        if (replaceTable.count(original) != 0 && replaceTable[original] != a[k])
          valid = false;

        // replacement successful
        replaceTable[original] = a[k];
      }

      // two digits must not map to the same letter, though
      std::set<char> used;
      for (auto x : replaceTable)
      {
        // already used ?
        if (used.count(x.second) != 0)
          valid = false;
        // mark as used
        used.insert(x.second);
      }

      // any constraint violation ?
      if (!valid)
        continue;

      // using that mapping, can "a" be constructed ?
      std::string aa;
      for (auto x : replaceA)
        aa += replaceTable[x];
      if (aa != a)
        continue;

      // using that mapping, can "b" be constructed ?
      std::string bb;
      for (auto x : replaceB)
        bb += replaceTable[x];
      if (bb != b)
        continue;

      // new bigger square ?
      if (result < i)
        result = i;
      if (result < j)
        result = j;
    }

  return result;
}

static string problem98()
{
    map<std::string, std::vector<std::string>> anagrams;
    ifstream ifs;
    ifs.open("p098_words.txt");

    while (true)
    {
        auto word = readWord(ifs);
        if (word.empty())
            break;

        auto sorted = word;
        std::sort(sorted.begin(), sorted.end());
        anagrams[sorted].push_back(word);
    }

    ifs.close();

  // find longest anagram
  size_t maxDigits = 0;
  for (auto i : anagrams)
    if (i.second.size() > 1) // at least two words share the same letters ?
      if (maxDigits < i.second.front().size())
        maxDigits = i.second.front().size();
  // maxDigits will be 9 for the given input ("INTRODUCE", "REDUCTION")

  unsigned long long maxNumber = 1;
  for (size_t i = 0; i < maxDigits; i++)
    maxNumber *= 10;

  // generate all squares
  // for each square, compute its fingerprint
  std::map<unsigned long long, std::vector<unsigned long long>> permutations;
  std::map<unsigned int,       std::set   <unsigned long long>> fingerprintLength;
  // walk through all square numbers (base^2)
  unsigned long long base = 1;
  while (base*base <= maxNumber)
  {
    auto square = base*base;
    auto id     = fingerprint(square);
    permutations[id].push_back(square);

    auto numDigits = log10(square - 1) + 1;
    fingerprintLength[numDigits].insert(id);

    base++;
  }

    uint64_t result = 0;
    for (auto i : anagrams)
    {
    auto pairs = i.second;
    // no other word with the same letters ?
    if (pairs.size() == 1)
      continue;

    auto length = pairs.front().size();
    // compare each word with each other
    for (size_t i = 0; i < pairs.size(); i++)
      for (size_t j = i + 1; j < pairs.size(); j++)
      {
        // extract all relevant squares
        for (auto id : fingerprintLength[length])
        {
          // and perform the matching process ...
          auto best = match(pairs[i], pairs[j], permutations[id]);
          // bigger square found ?
          if (result < best)
            result = best;
        }
      }
    }

    return twostring<uint64_t>(result);
}

#if 0
static string problem98()
{
    char *words = new char[2000*50];
    ifstream ifs;
    ifs.open("euler98.txt");
    char c;
    uint32_t word = 0, letter = 0;
    while (ifs.get(c))
    {   if (isalpha(c))
        {   words[word * 50 + letter] = c;
            letter++;
            continue;
        }
        words[word * 50 + letter] = 0;
        letter = 0;
        word++;
    }
    ifs.close();
#if 0
    for (uint32_t i = 0; i < word - 1; i++)
        cout << (words + i * 50) << "\r\n";
#endif
    delete[] words;
    return twostring<uint32_t>(0);
}
#endif

/*
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
*/

/*
https://euler.stephan-brumme.com/99/
*/

static string problem99()
{   ifstream ifs;
    ifs.open("euler99.txt");
    map<double, uint32_t> data;
    for (uint32_t i = 1; i <= 1000; i++) // first line has index 1 (not 0)
    {   uint32_t base, exponent;
        char comma; // skip commas in input file
        ifs >> base >> comma >> exponent;
        data[exponent * log(base)] = i;
    }
    ifs.close();
    return twostring<uint32_t>(data.rbegin()->second);
}

/*
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
*/

/*
https://blog.dreamshire.com/project-euler-100-solution/
*/

static string problem100()
{   uint64_t b = 3, n = 4, L = 1000000000000ULL;
    while (n <= L)
    {   uint64_t tmp = 3 * b + 2 * n - 2;
        n = 4 * b + 3 * n - 3;
        b = tmp;
    }
    return twostring<uint64_t>(b);
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
    case 63: return problem63();
    case 64: return problem64();
    case 65: return problem65();
    case 66: return problem66();
    case 67: return problem67();
    case 68: return problem68();
    case 69: return problem69();
    case 70: return problem70();
    case 71: return problem71();
    case 72: return problem72();
    case 73: return problem73();
    case 74: return problem74();
    case 75: return problem75();
    case 76: return problem76();
    case 77: return problem77();
    case 78: return problem78();
    case 79: return problem79();
    case 80: return problem80();
    case 81: return problem81();
    case 82: return problem82();
    case 83: return problem83();
    case 84: return problem84();
    case 85: return problem85();
    case 86: return problem86();
    case 87: return problem87();
    case 88: return problem88();
    case 89: return problem89();
    case 90: return problem90();
    case 91: return problem91();
    case 92: return problem92();
    case 93: return problem93();
    case 94: return problem94();
    case 95: return problem95();
    case 96: return problem96();
    case 97: return problem97();
    case 98: return problem98();
    case 99: return problem99();
    case 100: return problem100();
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
    "121313", "142857", "4075", "376", "249", "972", "153", "26241",
    "107359", "26033", "28684", "127035954683", "49", "1322", "272", "661", "7273",
    "6531031914842725", "510510", "8319823", "428570", "303963552391", "7295372", "402", "161667",
    "190569291", "71", "55374", "73162890", "40886", "427337", "260324", "425185",
    "101524", "2772", "1818", "1097343", "7587457", "743", "1217", "14234", "8581146",
    "1258", "518408346", "14316", "24702", "8739992577", "18769", "709", "756872327473",
    "37076114526", "228"};


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
        cout << "ERROR: ";

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
    testLongNum();
    time_t begin = time(0);
#ifdef MULTITHREAD
    multithread(100);
#else
    singlethread(100);
#endif
    time_t end = time(0);
    cout << "Total: " << end - begin << "s\r\n";
    return 0;
}


