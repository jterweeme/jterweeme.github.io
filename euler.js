function opdracht1(limit = 1000)
{   function summation(n, xmax)
    {   var xlen = xmax / n |0;
        return Math.floor((xlen * (xlen + 1)) / 2) * n;
    }
    return summation(3, limit - 1) + summation(5, limit - 1) - summation(15, limit - 1);
}

function opdracht2()
{   var xmax = 4*10**6, term1 = 1, term2 = 2, temp = 0, xsum = 2;
    while ((temp = term1 + term2) <= xmax)
    {   if (temp % 2 == 0) xsum += temp;
        term1 = term2, term2 = temp;
    }
    return xsum;
}

function opdracht3()
{   function primefactors(n)
    {   var primes = [], factors = [];
        function primefactor(primes, n = 13195)
        {   function appendprime(primes)
            {   if (primes.length < 1) primes.push(2);
                if (primes.length < 2) primes.push(3);
                var p = primes[primes.length - 1] + 2;
                for (var i = 2; i < p; i++) if (p % i == 0) p++, i = 1;
                primes.push(p);
                return primes;
            }
            for (var i = 0; true; i++)
            {   if (primes.length <= i) appendprime(primes);
                if (n % primes[i] == 0) return primes[i];
            }
        }
        while (true)
        {   var factor = primefactor(primes, n);
            factors.push(factor);
            if (factor == n) break;
            n = Math.floor(n / factor);
        }
        return factors
    }
    return Math.max.apply(null, primefactors(600851475143));
}

function opdracht4()
{   function ispalindrome(n)
    {   var temp = n, rev = 0;
        while (temp != 0)
            rev = rev * 10 + temp % 10, temp = Math.floor(temp / 10);
        return n == rev;
    }
    var best = 0;
    for (var a = 100; a < 1000; a++)
    {   for (var b = 100; b < 1000; b++)
        {   var c = a * b;
            if (ispalindrome(c) && c > best) best = c;
        }
    }
    return best;
}

function opdracht5()
{   function isdivisible(n, l)
    {   for (var i = 0; i < l.length; i++) if (n % l[i] > 0) return 0;
        return 1;
    }
    var start = 2520, number = start;
    while (isdivisible(number, [11,12,13,14,15,16,17,18,19,20]) == 0) number += start;
    return number;
}

function opdracht6()
{   var sumsquare = 0, squaresum = 0;
    for (var x = 1; x <= 100; x++)
        sumsquare += x**2, squaresum += x;
    return squaresum ** 2 - sumsquare;
}

function opdracht7()
{   function reducer(n, presets = [300, 100, 8])
    {   for (var i = 0; i < presets.length; i++)
            if (n > presets[i] * presets[i]) return n / presets[i] |0;
        return n;
    }
    var p = 3, sqp = reducer(p), ret = 0, n = 10001;
    for (var j = 0; j < n - 1; j++)
    {   for (var i = 2; i < sqp; i++) if (p % i == 0) sqp = reducer(++p), i = 1;
        ret = p, p += 2, sqp = reducer(p);
    }
    return ret;
}

console.log(opdracht1())
console.log(opdracht2())
console.log(opdracht3())
console.log(opdracht4())
console.log(opdracht5())
console.log(opdracht6())
console.log(opdracht7())

