using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

class Sieve
{
    private bool[] _sieve;
    //private int _limit;
    //private int _next;
    //private bool _hasNext;
    private List<UInt32> _primes;
    private int _pos = 0;

    public Sieve(int limit)
    {
        //_limit = limit;
        _sieve = new bool[limit];
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
    {   return _pos <= _primes.Count;
    }

    public UInt32 next()
    {   return _primes[_pos++];
    }
}

class Euler
{
    static UInt32 summation(UInt32 n, UInt32 max)
    {   UInt32 len = max / n;
        return ((len * (len + 1)) >> 1) * n;
    }

    static UInt32 problem1()
    {
        return summation(3, 999) + summation(5, 999) - summation(15, 999);
    }

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

    static UInt64 problem3(UInt64 n = 600851475143)
    {
        //Sieve sieve = new Sieve(9999);
        return 0;
    }

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

    static UInt32 problem6(UInt32 min = 1, UInt32 max = 100)
    {
        UInt32 sumsquare = 0, squaresum = 0;
        for (UInt32 i = min; i <= max; i++) sumsquare += i * i;
        for (UInt32 i = min; i <= max; i++) squaresum += i;
        squaresum = squaresum * squaresum;
        return squaresum - sumsquare;
    }

    static UInt32 problem39()
    {
        UInt32 best_p = 0, best_solutions = 0;
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

    static void Main()
    {
        Console.WriteLine(problem1());
        Console.WriteLine(problem2());
        Console.WriteLine(problem4());
        Console.WriteLine(problem5());
        Console.WriteLine(problem6());
        Console.WriteLine(problem39());
        Console.WriteLine(problem83());
    }
}


