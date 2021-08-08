using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

using static System.Console;
using static Codility.Codility;

namespace Codility
{
    static class Codility
    {
        public static int BinaryGap(int N)
        {
            int result = 0, count = -1;

            foreach (bool b in new BitArray(BitConverter.GetBytes(N)))
            {
                if (b)
                {
                    result = Math.Max(result, count);
                    count = 0;
                }
                else if (count >= 0)
                {
                    ++count;
                }
            }

            return result;
        }

        private static int GreatestCommonDivider(int a1, int a2)
        {
            int m = Math.Min(a1, a2);
            int n = Math.Max(a1, a2);

            while (n % m != 0)
            {
                int t = n % m;
                n = m;
                m = t;
            }

            return m;
        }

        public static int[] CyclicRotation(int[] A, int K)
        {
            if (A.Length > 0 && K > 0)
            {
                int gcd = GreatestCommonDivider(A.Length, K);

                if (gcd > 1 && gcd < A.Length)
                {
                    CyclicRotation(A, 1);
                    CyclicRotation(A, --K);
                }
                else
                {
                    for (int prev = A[0], count = 0, i = K % A.Length; count < A.Length; i = (i + K) % A.Length, ++count)
                    {
                        (A[i], prev) = (prev, A[i]);
                    }
                }
            }

            return A;
        }

        public static int OddOccurrencesInArray(int[] A)
        {
            Dictionary<int, int> occur = new Dictionary<int, int>();

            Array.ForEach(A, a => { occur[a] = occur.ContainsKey(a) ? ++occur[a] : 1; });

            return occur.FirstOrDefault(p => (p.Value & 1) != 0 ).Key;
        }

        public static int FrogJmp(int X, int Y, int D)
        {
            return (Y - X) / D + Convert.ToInt32((Y - X) % D != 0);//((Y - X) % D == 0 ? 0 : 1);
        }

        public static int PermMissingElem(int[] A)
        {
            HashSet<int> s = new HashSet<int>(A);

            for (int i = 1; i <= A.Length + 1; ++i)
            {
                if (!s.Contains(i))
                {
                    return i;
                }
            }

            return 0;
        }

        public static int TapeEquilibrium(int[] A)
        {
            int[] l = new int[A.Length - 1], r = new int[A.Length - 1];

            for (int i = 0; i < A.Length; ++i)
            {
                if (i < l.Length)
                {
                    l[i] = A[i] + (i != 0 ? l[i - 1] : 0);
                }

                if (i > 0)
                {
                    r[r.Length - i] = A[A.Length - i] + (i > 1 ? r[r.Length - i + 1] : 0);
                }
            }

            int res = int.MaxValue;

            for (int i = 0; i < l.Length && i < r.Length; ++i)
            {
                res = Math.Min(res, Math.Abs(l[i] - r[i]));
            }

            return res;
        }

        public static int FrogRiverOne(int X, int[] A)
        {
            HashSet<int> leaves = new HashSet<int>();

            for (int i = 0; i < A.Length; ++i)
            {
                leaves.Add(A[i]);

                if (leaves.Count == X)
                {
                    return i;
                }
            }

            return -1;
        }

        public static int[] MaxCounters(int N, int[] A)
        {
            int[] res = new int[N];

            int curMax = 0, setMax = 0;

            Array.ForEach(A, i =>
            {
                if (i <= 0 || i > N)
                {
                    setMax = curMax;
                }
                else
                {
                    res[i - 1] = Math.Max(res[i - 1], setMax);
                    curMax = Math.Max(curMax, ++res[i - 1]);
                }
            });

            for (int i = 0; i < res.Length; ++i)
            {
                res[i] = Math.Max(res[i], setMax);
            }

            return res;
        }

        public static int MissingInteger(int[] A)
        {
            HashSet<int> s = new HashSet<int>(A);

            for (int i = 1; i <= s.Count + 1; ++i)
            {
                if (!s.Contains(i))
                {
                    return i;
                }
            }

            return 1;
        }

        public static int PermCheck(int[] A)
        {
            HashSet<int> s = new HashSet<int>(A);

            if (s.Count == A.Length)
            {
                for (int i = 1; i <= A.Length; ++i)
                {
                    if (!s.Contains(i))
                    {
                        return 0;
                    }
                }

                return 1;
            }

            return 0;
        }

        public static int CountDiv(int A, int B, int K)
        {
            return B / K - A / K + (A % K == 0 ? 1 : 0); //Convert.ToInt32(A % K == 0);
        }

        public static int[] GenomicRangeQuery(string S, int[] P, int[] Q)
        {
            var def = new Dictionary<char, int> { ['A'] = 1, ['C'] = 2, ['G'] = 3, ['T'] = 4 };

            var imp = new int[def.Count, S.Length];

            for (int i = 0; i < S.Length; ++i)
            {
                for (int j = 0; j < imp.GetLength(0); ++j)
                {
                    imp[j, i] = (i == 0 ? 0 : imp[j, i - 1]) + (S[i] == def.ElementAt(j).Key ? 1 : 0);
                }
            }

            var res = new int[Math.Min(P.Length, Q.Length)];

            for (int i = 0; i < res.Length; ++i)
            {
                for (int j = 0; j < imp.GetLength(0); ++j)
                {
                    if (S[P[i]] == def.ElementAt(j).Key || imp[j, Q[i]] - imp[j, P[i]] > 0)
                    {
                        res[i] = def.ElementAt(j).Value;
                        break;
                    }
                }
            }

            return res;
        }

        public static int MinAvgTwoSlice(int[] A)
        {
            int min = int.MaxValue, idx = -1;

            for (int i = 0; i < A.Length - 1; ++i)
            {
                int s = Math.Min((A[i] + A[i + 1]) * 3, i < A.Length - 2 ? (A[i] + A[i + 1] + A[i + 2]) * 2 : min);

                if (s < min)
                {
                    min = s;
                    idx = i;
                }
            }

            return idx;
        }

        public static int PassingCars(int[] A)
        {
            int res = 0, zeros = 0;

            for (int i = 0; i < A.Length; ++i)
            {
                if (A[i] == 0)
                {
                    ++zeros;
                }
                else
                {
                    res += zeros;

                    if (res > 1000000000)
                    {
                        return -1;
                    }
                }
            }

            return res;
        }

        public static int Distinct(int[] A)
        {
            return (new HashSet<int>(A)).Count;
        }

        public static int MaxProductOfThree(int[] A)
        {
            Array.Sort(A);

            int res = int.MinValue;

            if (A[0] < 0 && A[1] < 0)
            {
                res = A[0] * A[1] * A[A.Length - 1];
            }

            return Math.Max(res, A[A.Length - 3] * A[A.Length - 2] * A[A.Length - 1]);
        }

        /*class PointData
        {
            public int Starts { get; set; } = 0;
            public int Ends { get; set; } = 0;
            public PointData AddStart()
            {
                ++Starts;
                return this;
            }
            public PointData AddEnd()
            {
                ++Ends;
                return this;
            }
        }*/

        public static int NumberOfDiscIntersections(int[] A)
        {
            /*var points = new SortedDictionary<long, PointData>();

            for (long i = 0; i < A.Length; ++i)
            {
                points[i - A[i]] = points.ContainsKey(i - A[i]) ? points[i - A[i]].AddStart() : new PointData { Starts = 1 };
                points[i + A[i]] = points.ContainsKey(i + A[i]) ? points[i + A[i]].AddEnd() : new PointData { Ends = 1 };
            }

            int count = 0, res = 0;

            foreach (var point in points.Select(p => p.Value))
            {
                res += point.Starts * (point.Starts - 1) / 2 + point.Starts * count;

                if (res > 10000000)
                {
                    return -1;
                }

                count += point.Starts - point.Ends;
            }

            return res;*/

            /*var points = new SortedDictionary<long, Tuple<int, int>>();

            for (long i = 0; i < A.Length; ++i)
            {
                var p = points.ContainsKey(i - A[i]) ? points[i - A[i]] : new Tuple<int, int>(0, 0);
                points[i - A[i]] = new Tuple<int, int>(p.Item1 + 1, p.Item2);

                p = points.ContainsKey(i + A[i]) ? points[i + A[i]] : new Tuple<int, int>(0, 0);
                points[i + A[i]] = new Tuple<int, int>(p.Item1, p.Item2 + 1);
            }

            int count = 0, res = 0;

            foreach (var p in points)
            {
                res += p.Value.Item1 * (p.Value.Item1 - 1) / 2 + p.Value.Item1 * count;

                if (res > 10000000)
                {
                    return -1;
                }

                count += p.Value.Item1 - p.Value.Item2;
            }

            return res;*/

            var pts = new SortedDictionary<long, (int Starts, int Ends)>();

            for (long i = 0; i < A.Length; ++i)
            {
                /*(int Starts, int Ends) p = points.ContainsKey(i - A[i]) ? points[i - A[i]] : (0, 0);
                points[i - A[i]] = (++p.Starts, p.Ends);*/

                pts[i - A[i]] = pts.ContainsKey(i - A[i]) ? (pts[i - A[i]].Starts + 1, pts[i - A[i]].Ends) : (1, 0);

                /*p = points.ContainsKey(i + A[i]) ? points[i + A[i]] : (0, 0);
                points[i + A[i]] = (p.Starts, ++p.Ends);*/

                pts[i + A[i]] = pts.ContainsKey(i + A[i]) ? (pts[i + A[i]].Starts, pts[i + A[i]].Ends + 1) : (0, 1);
            }

            int count = 0, res = 0;

            foreach (var pt in pts.Select(p => p.Value))
            {
                res += pt.Starts * (pt.Starts - 1) / 2 + pt.Starts * count;

                if (res > 10000000)
                {
                    return -1;
                }

                count += pt.Starts - pt.Ends;
            }

            return res;
        }

        public static int Triangle(int[] A)
        {
            if (A.Length > 2)
            {
                Array.Sort(A);

                for (int i = 0; i < A.Length - 2; ++i)
                {
                    if ((long)A[i] + A[i + 1] > A[i + 2])
                    {
                        return 1;
                    }
                }
            }

            return 0;
        }

        public static int Brackets(string S)
        {
            var brs= new Dictionary<char, char> { ['('] = ')', ['['] = ']', ['{'] = '}' };
            var stk = new Stack<char>();

            foreach (char c in S)
            {
                if (brs.ContainsKey(c))
                {
                    stk.Push(c);
                }
                else if (stk.Count > 0 && brs.ContainsKey(stk.Peek()) && brs[stk.Peek()] == c)
                {
                    stk.Pop();
                }
                else
                {
                    return 0;
                }
            }

            return stk.Count == 0 ? 1 : 0;
        }

        public static int Fish(int[] A, int[] B)
        {
            var fish = new Stack<(int Size, int Direct)/*Tuple<int, int>*/>();

            for (int i = 0; i < Math.Min(A.Length, B.Length); ++i)
            {
                if (fish.Count == 0 || B[i] == 1 || fish.Peek().Direct/*Item2*/ == B[i])
                {
                    fish.Push((A[i], B[i])/*new Tuple<int, int>(A[i], B[i])*/);
                }
                else if (fish.Peek().Direct/*Item2*/ != B[i] && fish.Peek().Size/*Item1*/ < A[i])
                {
                    fish.Pop();
                    --i;
                }
            }

            return fish.Count;
        }

        public static int Nesting(string S)
        {
            int count = 0;

            foreach (char c in S)
            {
                count += c == '(' ? 1 : -1;

                if (count < 0)
                {
                    return 0;
                }
            }

            return count == 0 ? 1 : 0;
        }

        public static int StoneWall(int[] H)
        {
            int res = 0;
            var stk = new Stack<int>();

            for (int i = 0; i < H.Length; ++i)
            {
                if (stk.Count == 0 || H[i] > stk.Peek())
                {
                    stk.Push(H[i]);
                    res++;
                }
                else if (H[i] < stk.Peek())
                {
                    stk.Pop();
                    --i;
                }
            }

            return res;
        }

        public static int MaxSliceSum(int[] A)
        {
            int res = int.MinValue;

            if (A.Length > 0)
            {
                int sum = A[0];
                res = sum;

                foreach (int a in A.Skip(1))
                {
                    sum = Math.Max(sum + a, a);
                    res = Math.Max(res, sum);
                }
            }

            return res;
        }

        public static int MaxProfit(int[] A)
        {
            if (A.Length > 0)
            {
                for (int i = 0, prev = A[0]; i < A.Length; ++i)
                {
                    int temp = A[i];
                    A[i] -= prev;
                    prev = temp;
                }

                return MaxSliceSum(A);
            }

            return 0;
        }
    }
    
    class Program
    {
        static void Main(string[] args)
        {
            //WriteLine(BinaryGap(1041));

            //WriteLine(string.Join(" ", CyclicRotation(new int[] { 1, 2, 3, 4, 5, 6 }, 3)));

            //WriteLine(OddOccurrencesInArray(new int[] { 9,3,9,3,9,7,9 }));

            //WriteLine(FrogJmp(10, 85, 30));

            //WriteLine(PermMissingElem(new int[] { 2,3,1,5 }));

            //WriteLine(TapeEquilibrium(new int[] { 3,1,2,4,3 }));

            //WriteLine(FrogRiverOne(5, new int[] { 1,3,1,4,2,3,5,4 }));

            //WriteLine(string.Join(" ", MaxCounters(5, new int[] { 3,4,4,6,1,4,4 })));

            //WriteLine(MissingInteger(new int[] { 1, 3, 6, 4, 1, 2 }));

            //WriteLine(PermCheck(new int[] { 4,1,3,2 }));

            //WriteLine(CountDiv(6, 11, 2));

            //WriteLine(string.Join(" ", GenomicRangeQuery("CAGCCTA", new int[] { 2,5,0 }, new int[] { 4,5,6 })));

            //WriteLine(MinAvgTwoSlice(new int[] { 4,2,2,5,1,5,8 }));

            //WriteLine(PassingCars(new int[] { 0,1,0,1,1 }));

            //WriteLine(Distinct(new int[] { 2,1,1,2,3,1 }));

            //WriteLine(MaxProductOfThree(new int[] { -3,1,2,-2,5,6 }));

            //WriteLine(NumberOfDiscIntersections(new int[] { 1,5,2,1,4,0 }));

            //WriteLine(Triangle(new int[] { 10,2,5,1,8,20 }));

            //WriteLine(Brackets("{[()()]}"));

            //WriteLine(Fish(new int[] { 4,3,2,1,5 }, new int[] { 0,1,0,0,0 }));

            //WriteLine(Nesting("(()(()()))"));

            //WriteLine(StoneWall(new int[] { 8,8,5,7,9,8,7,4,8 }));

            //WriteLine(MaxSliceSum(new int[] { 3,2,-6,4,0 }));

            //WriteLine(MaxProfit(new int[] { 23171, 21011, 21123, 21366, 21013, 21367 }));
        }
    }
}
