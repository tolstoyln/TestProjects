#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <unordered_set>
#include <set>
#include <map>
#include <stack>
#include <numeric>
//#include <list>

#pragma warning(push, 4)

int BinaryGap(int n)
{
    int result = 0, count = -1;

    for (size_t i = 0; i < sizeof(n) * std::numeric_limits<unsigned char>::digits; ++i)
    {
        if (((n >> i) & 1) == 0)
        {
            if (count >= 0)
            {
                ++count;
            }
        }
        else
        {
            result = std::max(result, count);
            count = 0;
        }
    }

    return result;
}

void CyclicShiftRight(std::vector<int>& v)
{
    if (v.size() > 0)
    {
        int next = v[0];

        for (size_t i = 1; i < v.size(); ++i)
        {
            int tmp = v[i];
            v[i] = next;
            next = tmp;
        }

        v[0] = next;
    }
}

std::vector<int>& CyclicRotation(std::vector<int>& v, int count)
{
    while (--count >= 0)
    {
        CyclicShiftRight(v);
    }

    return v;
}

int OddOccurrencesInArray(const std::vector<int>& A)
{
    std::unordered_map<int, int> m;

    std::for_each(A.begin(), A.end(), [&m](int i)
        {
            ++m[i];
        });

    return std::find_if(m.begin(), m.end(), [](const auto& it) {return (it.second & 1) != 0; })->first;
}

int FrogJmp(int X, int Y, int D)
{
    return (Y - X) / D + ((Y - X) % D == 0 ? 0 : 1);
}

intptr_t PermMissingElem(const std::vector<int>& A)
{
    std::vector<bool> v(A.size() + 1, false);

    std::for_each(A.begin(), A.end(), [&v](size_t i) { v[i - 1] = true; });

    return std::distance(v.begin(), std::find_if(v.begin(), v.end(), [](bool b) { return !b; })) + 1;
}

int TapeEquilibrium(const std::vector<int>& A)
{
    std::vector<int> l(A.size() - 1);

    for (size_t i = 0; i < A.size() - 1; ++i)
    {
        l[i] = A[i] + (i > 0 ? l[i - 1] : 0);
    }

    std::vector<int> r(A.size() - 1);

    for (size_t i = A.size() - 1; i > 0; --i)
    {
        r[i - 1] = A[i] + ((i < A.size() - 1) ? r[i] : 0);
    }

    int result = std::numeric_limits<decltype(result)>::max();

    for (size_t i = 0; i < l.size(); ++i)
    {
        int dif = std::abs(l[i] - r[i]);
        result = std::min(result, dif);
    }

    return result;
}

intptr_t FrogRiverOne(int X, const std::vector<int>& A)
{
    std::unordered_set<int> s;

    for (size_t i = 0; i < A.size(); ++i)
    {
        s.insert(A[i]);

        if (s.size() == static_cast<size_t>(X))
        {
            return i;
        }
    }

    return -1;
}

std::vector<int> MaxCounters(int N, const std::vector<int>& A)
{
    std::vector<int> v(N);
    int curMaxVal = 0, setMaxVal = 0;

    std::for_each(A.begin(), A.end(), [&](intptr_t i)
        {
            if (i > N || i <= 0)
            {
                setMaxVal = curMaxVal;
            }
            else
            {
                v[i - 1] = std::max(v[i - 1], setMaxVal);
                ++v[i - 1];
                curMaxVal = std::max(curMaxVal, v[i - 1]);
            }
        });

    std::for_each(v.begin(), v.end(), [&setMaxVal](int& i) { i = std::max(i, setMaxVal); });

    return v;
}

intptr_t MissingInteger(const std::vector<int>& A)
{
    std::unordered_set<size_t> s(A.begin(), A.end());

    for (size_t i = 1; i <= s.size() + 1; ++i)
    {
        if (s.find(i) == s.end())
        {
            return i;
        }
    }

    return 0;
}

int PermCheck(const std::vector<int>& A)
{
    std::unordered_set<size_t> s(A.begin(), A.end());

    if (s.size() != A.size())
    {
        return 0;
    }

    for (size_t i = 1; i <= A.size(); ++i)
    {
        if (s.find(i) == s.end())
        {
            return 0;
        }
    }

    return 1;
}

int CountDiv(int A, int B, int K)
{
    return B / K - A / K + (A % K == 0 ? 1 : 0);
}

#pragma warning(pop)

std::vector<int> GenomicRangeQuery(const std::string& S, const std::vector<int>& P, const std::vector<int>& Q)
{
    /*std::unordered_map<char, int> def { {'A', 1}, {'C', 2}, {'G', 3}, {'T', 4} };

    std::vector<int> imp(S.length());
    std::transform(S.begin(), S.end(), imp.begin(), [&def](char c) { return def[c]; });

    std::vector<int> res(P.size());

    for (size_t i = 0; i < P.size(); ++i)
    {
        res[i] = *std::min_element(imp.begin() + P[i], imp.begin() + Q[i] + 1);
    }*/

    std::unordered_map<char, int> def{ {'A', 1}, {'C', 2}, {'G', 3}, {'T', 4} };

    std::vector<std::vector<int>> imp(def.size(), std::vector<int>(S.length()));

    for (size_t i = 0; i < S.length(); ++i)
    {
        for (size_t j = 0; j < imp.size(); ++j)
        {
            imp[j][i] = (i > 0 ? imp[j][i - 1] : 0) + (std::next(def.begin(), j)->first == S[i] ? 1 : 0);
        }
    }

    std::vector<int> res(P.size());

    for (size_t i = 0; i < P.size(); ++i)
    {
        for (size_t j = 0; j < imp.size(); ++j)
        {
            if (S[P[i]] == std::next(def.begin(), j)->first || imp[j][Q[i]] - imp[j][P[i]] > 0)
            {
                res[i] = std::next(def.begin(), j)->second;
                break;
            }
        }
    }

    return res;
}

intptr_t MinAvgTwoSlice(const std::vector<int>& A)
{
    int minSum = std::numeric_limits<decltype(minSum)>::max();
    size_t idx = 0;

    for (size_t i = 0; i < A.size() - 1; ++i)
    {
        int sum = std::min((A[i] + A[i + 1]) * 3, (i < A.size() - 2) ? (A[i] + A[i + 1] + A[i + 2]) * 2 : std::numeric_limits<decltype(minSum)>::max());

        if (sum < minSum)
        {
            minSum = sum;
            idx = i;
        }
    }

    return idx;
}

int PassingCars(const std::vector<int>& A)
{
    int zeros = 0;
    int result = 0;

    for (size_t i = 0; i < A.size(); ++i)
    {
        if (A[i] == 0)
        {
            ++zeros;
        }
        else
        {
            result += zeros;

            if (result > 1000000000)
            {
                return -1;
            }
        }
    }

    return result;
}

intptr_t Distinct(const std::vector<int>& A)
{
    return std::unordered_set<int>(A.begin(), A.end()).size();
}

int MaxProductOfThree(const std::vector<int>& A)
{
    std::multiset<int> s(A.begin(), A.end());

    auto itl = s.begin();
    auto itr = --s.end();

    int pl = std::numeric_limits<decltype(pl)>::min();

    if (*itl < 0)
    {
        pl = (*itl) * (*++itl) * (*itr);
    }

    return std::max(pl, (*itr) * (*--itr) * (*--itr));
}

int NumberOfDiscIntersections(const std::vector<int>& A)
{
    if (A.size() < 2)
    {
        return 0;
    }

    std::map<long long, std::pair<int, int>> m;

    for (size_t i = 0; i < A.size(); ++i)
    {
        ++m[static_cast<long long>(i) - A[i]].first;
        ++m[static_cast<long long>(i) + A[i]].second;
    }

    int count = 0, result = 0;

    for (auto it = m.begin(); it != m.end(); ++it)
    {
        result += it->second.first * (it->second.first - 1) / 2 + it->second.first * count;

        if (result > 10000000)
        {
            return -1;
        }

        count += it->second.first - it->second.second;
    }

    return result;
}

int Triangle(const std::vector<int>& A)
{
    if (A.size() >= 3)
    {
        std::multiset<long long> s(A.begin(), A.end());

        for (auto it = s.begin(); it != std::prev(s.end(), 2); std::advance(it, 1))
        {
            if (*it + *std::next(it) > *std::next(it, 2))
            {
                return 1;
            }
        }
    }

    return 0;
}

int Brackets(const std::string& S)
{
    std::unordered_map<char, char> br{ {'(',')'},  {'[',']'}, {'{','}'} };
    std::stack<char> s;

    for (char c : S)
    {
        if (br.find(c) != br.end())
        {
            s.push(c);
        }
        else
        {
            if (s.size() > 0 && br[s.top()] == c)
            {
                s.pop();
            }
            else
            {
                return 0;
            }
        }
    }

    return s.empty() ? 1 : 0;
}

intptr_t Fish(const std::vector<int>& A, const std::vector<int>& B)
{
    std::stack<std::pair<int, int>> s;

    for (size_t i = 0; i < A.size(); ++i)
    {
        if (s.empty() || B[i] == 1 || B[i] == s.top().second)
        {
            s.push(std::pair<int, int>(A[i], B[i]));
        }
        else if (B[i] == 0 && A[i] > s.top().first)
        {
            s.pop();
            --i;
        }
    }

    return s.size();
}

int Nesting(const std::string& S)
{
    int count = 0;

    for (char c : S)
    {
        count += c == '(' ? 1 : -1;

        if (count < 0)
        {
            return 0;
        }
    }

    return count == 0 ? 1 : 0;
}

int StoneWall(const std::vector<int>& H)
{
    int count = 0;
    std::stack<int> h;

    for (size_t i = 0; i < H.size(); ++i)
    {
        if (h.empty() || H[i] > h.top())
        {
            h.push(H[i]);
            ++count;
        }
        else if (H[i] < h.top())
        {
            h.pop();
            --i;
        }
    }

    return count;
}

intptr_t Dominator(const std::vector<int>& A)
{
    std::stack<int> s;

    std::for_each(A.begin(), A.end(), [&s](int i)
        {
            if (s.empty() || i == s.top())
            {
                s.push(i);
            }
            else
            {
                s.pop();
            }
        });

    if (!s.empty())
    {
        for (size_t i = 0, count = 0; i < A.size(); ++i)
        {
            if (A[i] == s.top())
            {
                if (++count > A.size() / 2)
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

int EquiLeader(const std::vector<int>& A)
{
    std::stack<int> s;

    std::for_each(A.begin(), A.end(), [&s](int i)
        {
            if (s.empty() || i == s.top())
            {
                s.push(i);
            }
            else
            {
                s.pop();
            }
        });

    int result = 0;

    if (!s.empty())
    {
        std::vector<size_t> v(A.size());
        size_t count = 0;

        for (size_t i = 0; i < A.size(); ++i)
        {
            if (A[i] == s.top())
            {
                ++count;
            }

            v[i] = count;
        }

        if (count > A.size() / 2)
        {
            for (size_t i = 0; i < v.size(); ++i)
            {
                if (v[i] > (i + 1) / 2 && v[A.size() - 1] - v[i] > (A.size() - i - 1) / 2)
                {
                    ++result;
                }
            }
        }
    }

    return result;
}

int MaxSliceSum(const std::vector<int>& A, int resultForEmpty = 0)
{
    int result = resultForEmpty;

    if (!A.empty())
    {
        int curMax = A[0];
        result = curMax;

        std::for_each(++A.begin(), A.end(), [&](int i)
            {
                curMax = std::max(i, curMax + i);
                result = std::max(result, curMax);
            });
    }

    return result;
}

int MaxProfit(const std::vector<int>& A)
{
    std::vector<int> v(A.size());

    for (size_t i = 0; i < A.size(); ++i)
    {
        v[i] = i > 0 ? A[i] - A[i - 1] : 0;
    }

    return MaxSliceSum(v);
}

int MaxDoubleSliceSum(const std::vector<int>& A)
{
    std::vector<int> l(A.size(), 0);

    for (size_t i = 1; i < A.size() - 1; ++i)
    {
        l[i] = std::max(0, l[i - 1] + A[i]);
    }

    std::vector<int> r(A.size(), 0);

    for (size_t i = A.size() - 2; i > 0; --i)
    {
        r[i] = std::max(0, r[i + 1] + A[i]);
    }

    int result = 0;

    for (size_t i = 1; i < A.size() - 1; ++i)
    {
        result = std::max(result, l[i - 1] + r[i + 1]);
    }

    return result;
}

int CountFactors(int N)
{
    int count = 0, i = 0;
    double max = std::sqrt(N);

    while (++i <= static_cast<int>(max))
    {
        if (N % i == 0)
        {
            count += (i == max) ? 1 : 2;
        }
    }

    return count;
}

int MinPerimeterRectangle(int N)
{
    int result = std::numeric_limits<decltype(result)>::max();
    int max = static_cast<int>(std::sqrt(N));
    int i = 0;

    while (++i <= max)
    {
        if (N % i == 0)
        {
            result = std::min(result, 2 * (i + N / i));
        }
    }

    return result;
}

intptr_t Flags(const std::vector<int>& A)
{
    if (A.size() < 3)
    {
        return 0;
    }

    std::vector<intptr_t> peaks;
    peaks.reserve(A.size() - 2);

    for (size_t i = 1; i < A.size() - 1; ++i)
    {
        if (A[i - 1] < A[i] && A[i] > A[i + 1])
        {
            peaks.push_back(i);
        }
    }

    if (peaks.size() < 2)
    {
        return peaks.size();
    }

    for (intptr_t i = static_cast<intptr_t>(std::sqrt(peaks[peaks.size() - 1] - peaks[0])) + 1; i > 1; --i)
    {
        for (intptr_t j = 0, sum = 0, flags = 1; j < static_cast<intptr_t>(peaks.size()) - 1; ++j)
        {
            if (sum + peaks[j + 1] - peaks[j] >= i)
            {
                ++flags;
                sum = 0;
            }
            else
            {
                sum += peaks[j + 1] - peaks[j];
            }

            if (flags == i)
            {
                return flags;
            }
        }
    }

    return 0;
}

int Peaks(const std::vector<int>& A)
{
    if (A.size() >= 3)
    {
        std::vector<int> peaks(A.size(), 0);

        for (intptr_t i = 1; i < static_cast<decltype(i)>(A.size()) - 1; ++i)
        {
            if (A[i - 1] < A[i] && A[i] > A[i + 1])
            {
                peaks[i] = 1;
            }
        }

        if (std::find(peaks.begin(), peaks.end(), 1) != peaks.end())
        {
            for (intptr_t i = A.size() / 2; i > 0; --i)
            {
                if (A.size() % i == 0 && i <= static_cast<decltype(i)>(peaks.size()))
                {
                    intptr_t size = A.size() / i;
                    int blocks = 0;

                    for (intptr_t j = 0; j < i; ++j)
                    {
                        if (std::find(peaks.data() + size * j, peaks.data() + size * (j + 1), 1) == peaks.data() + size * (j + 1))
                        {
                            break;
                        }

                        ++blocks;
                    }

                    if (blocks == i)
                    {
                        return blocks;
                    }
                }
            }
        }
    }

    return 0;
}

std::vector<int> CountNonDivisible(const std::vector<int>& A)
{
    std::unordered_map<int, int> count;
    std::for_each(A.begin(), A.end(), [&](int a) { ++count[a]; });

    std::vector<int> result(A.size(), static_cast<int>(A.size()));

    for (size_t i = 0; i < A.size(); ++i)
    {
        double max = std::sqrt(A[i]);

        for (int j = 1; j <= static_cast<int>(max); ++j)
        {
            if (A[i] % j == 0)
            {
                result[i] -= count[j] + (j != max ? count[A[i] / j] : 0);
            }
        }
    }

    return result;
}

bool _IsCube(int n)
{
    double r = std::cbrt(n);
    return static_cast<int>(r) * static_cast<int>(r) * static_cast<int>(r) == n;
}

std::vector<int> CountSemiprimes(int N, const std::vector<int>& P, const std::vector<int>& Q)
{
    std::vector<int> sp(static_cast<size_t>(N) + 1, 0);

    for (int i = 1; i <= N; ++i)
    {
        int divs = CountFactors(i);
        sp[i] = sp[static_cast<size_t>(i) - 1] + ((divs == 3 || divs == 4 && !_IsCube(i)) ? 1 : 0);
    }

    std::vector<int> res(P.size(), 0);

    for (size_t i = 0; i < P.size(); ++i)
    {
        res[i] = sp[Q[i]] - sp[static_cast<size_t>(P[i]) - 1];
    }

    return res;
}

int GreatestCommonDivider(int a1, int a2)
{
    int m = std::min(a1, a2);
    int n = std::max(a1, a2);

    while (n % m != 0)
    {
        int t = n % m;
        n = m;
        m = t;
    }

    return m;
}

int ChocolatesByNumbers(int N, int M)
{
    /*std::vector<bool> v(N, false);
    int count = 0;

    for (int i = 0; !v[i]; i = (i + M) % N)
    {
        v[i] = true;
        ++count;
    }

    return count;*/

    return N / GreatestCommonDivider(N, M);
}

bool NHasAllPrimeDivisorsOfM(int m, int n)
{
    if (m == 1)
    {
        return true;
    }
    int gcd = GreatestCommonDivider(m, n);
    return gcd > 1 && NHasAllPrimeDivisorsOfM(m / gcd, n);
}

int CommonPrimeDivisors(const std::vector<int>& A, const std::vector<int>& B)
{
    int count = 0;

    for (size_t i = 0; i < A.size(); ++i)
    {
        count += (NHasAllPrimeDivisorsOfM(A[i], B[i]) && NHasAllPrimeDivisorsOfM(B[i], A[i])) ? 1 : 0;
    }

    return count;
}

intptr_t FibonacciAt(intptr_t idx)
{
    std::vector<intptr_t> f(idx + 1, 0);
    f[1] = 1;

    for (intptr_t i = 2; i <= idx; ++i)
    {
        f[i] = f[i - 2] + f[i - 1];
    }

    return f[idx];
}

std::vector<intptr_t> FibonacciUpTo(intptr_t n)
{
    std::vector<intptr_t> f{ 1, 2 };
    size_t i = 1;

    while (f[i++] < n)
    {
        f.push_back(f[i - 2] + f[i - 1]);
    }

    return f;
}

intptr_t FibFrog(std::vector<int>& A)
{
    A.insert(A.begin(), 1);
    A.push_back(1);

    std::vector<intptr_t> fib = FibonacciUpTo(A.size());

    std::vector<intptr_t> jmps(A.size(), A.size());
    jmps[0] = 0;

    for (intptr_t i = 1; i < static_cast<decltype(i)>(A.size()); ++i)
    {
        if (A[i] == 1)
        {
            for (size_t j = 0; j < fib.size(); ++j)
            {
                if (i - fib[j] >= 0)
                {
                    jmps[i] = std::min(jmps[i], jmps[i - fib[j]] + 1);
                }
                else
                {
                    break;
                }
            }
        }
    }

    return jmps[jmps.size() - 1] < static_cast<int>(A.size()) ? jmps[jmps.size() - 1] : -1;
}

std::vector<int> Ladder(const std::vector<int>& A, const std::vector<int>& B)
{
    intptr_t maxIdx = *std::max_element(A.begin(), A.end());
    std::vector<unsigned long long> fib(maxIdx + 1, 1);

    for (intptr_t i = 2; i <= maxIdx; ++i)
    {
        fib[i] = fib[i - 2] + fib[i - 1];
    }

    std::vector<int> res(A.size());

    for (size_t i = 0; i < A.size(); ++i)
    {
        res[i] = static_cast<int>(fib[A[i]] % (1ULL << B[i]));
    }

    return res;
}

int MinMaxDivision(int K, int M, const std::vector<int>& A)
{
    int min = *std::max_element(A.begin(), A.end());

    if (K >= static_cast<int>(A.size()))
    {
        return min;
    }

    int max = std::accumulate(A.begin(), A.end(), 0);

    if (K == 1)
    {
        return max;
    }

    while (min <= max)
    {
        int mdl = (min + max) / 2, blocks = 1;

        for (int i = 0, sum = 0; i < static_cast<decltype(i)>(A.size()); ++i)
        {
            sum += A[i];

            if (sum > mdl)
            {
                sum = A[i];
                ++blocks;
            }
        }

        if (blocks <= K)
        {
            max = --mdl;
        }
        else
        {
            min = ++mdl;
        }
    }

    return min;
}

intptr_t NailingPlanks(std::vector<int>& A, std::vector<int>& B, const std::vector<int>& C)
{
    /*std::list<std::pair<int, int>> planks;
    std::transform(A.begin(), A.end(), B.begin(), std::back_inserter(planks),
        [](int a, int b) { return std::make_pair(a, b); });

    for (size_t min = 1, count = 0; count < C.size();)
    {
        size_t nails = (C.size() + min) / 2;

        for (size_t i = count; i < nails; ++i)
        {
            ++count;

            for (auto it = planks.begin(); it != planks.end();)
            {
                if (C[i] >= it->first && C[i] <= it->second)
                {
                    it = planks.erase(it);

                    if (planks.empty())
                    {
                        return count;
                    }
                }
                else
                {
                    ++it;
                }
            }
        }

        min = ++nails;
    }*/

    /*for (size_t min = 1, count = 0; count < C.size();)
    {
        size_t nails = (C.size() + min) / 2;

        for (; count < nails; ++count)
        {
            for (size_t i = 0; i < A.size(); ++i)
            {
                if (C[count] >= A[i] && C[count] <= B[i])
                {
                    A.erase(A.begin() + i);
                    B.erase(B.begin() + i--);

                    if (A.empty())
                    {
                        return ++count;
                    }
                }
            }
        }

        min = ++nails;
    }

    return -1;*/

    size_t min = 1, max = C.size();

    while (min <= max && min <= C.size())
    {
        size_t cur = (min + max) / 2;

        std::unordered_set<intptr_t> pos(C.begin(), C.begin() + cur);

        std::vector<int> nails(static_cast<size_t>(*std::max_element(B.begin(), B.end())) + 1, 0);

        for (size_t i = 1; i < nails.size(); ++i)
        {
            nails[i] = nails[i - 1] + (pos.find(i) == pos.end() ? 0 : 1);
        }

        size_t plank = 0;

        for (; plank < A.size(); ++plank)
        {
            if (nails[B[plank]] - nails[static_cast<size_t>(A[plank]) - 1] == 0)
            {
                break;
            }
        }

        if (plank == A.size())
        {
            if (min == max)
            {
                break;
            }
            max = cur;
        }
        else
        {
            min = ++cur;
        }
    }

    return min <= C.size() ? min : -1;
}

intptr_t AbsDistinct(const std::vector<int>& A)
{
    std::unordered_set<int> s;

    std::for_each(A.begin(), A.end(), [&s](int i) { return s.insert(std::abs(i)); });

    return s.size();
}

int CountDistinctSlices(int M, const std::vector<int>& A)
{
    /*int count = 0;

    for (size_t i = 0; i < A.size(); ++i)
    {
        for (size_t j = i; j < A.size(); ++j)
        {
            std::unordered_set<int> s(A.begin() + i, A.begin() + j + 1);
            if (s.size() == j - i + 1)
            {
                ++count;

                if (count > 1000000000)
                {
                    return 1000000000;
                }
            }
            else
            {
                break;
            }
        }
    }

    return count;*/

    std::vector<int> prevs(*std::max_element(A.begin(), A.end()) + 1, -1);

    int result = 0;

    for (int cur = 0, prev = -1; cur < static_cast<int>(A.size()); prevs[A[cur]] = cur++)
    {
        if (prevs[A[cur]] > prev)
        {
            prev = prevs[A[cur]];
        }

        result += cur - prev;

        if (result > 1000000000)
        {
            return 1000000000;
        }
    }

    return result;
}

int CountTriangles(std::vector<int>& A)
{
    std::sort(A.begin(), A.end());

    int result = 0;

    for (int r = static_cast<int>(A.size()) - 1; r > 0; --r)
    {
        for (int l = 0, i = r - 1; l < i; ++l)
        {
            if (A[l] + A[i] > A[r])
            {
                result += i-- - l--;
            }
        }
    }

    return result;
}

int MinAbsSumOfTwo(std::vector<int>& A)
{
    std::sort(A.begin(), A.end());

    int res = std::numeric_limits<decltype(res)>::max();

    for (int l = 0, r = static_cast<int>(A.size()) - 1; l <= r;)
    {
        int sum = A[l] + A[r];

        res = std::min(res, std::abs(sum));

        if (sum < 0)
        {
            ++l;
        }
        else if (sum > 0)
        {
            --r;
        }
        else
        {
            break;
        }
    }

    return res;
}

int MaxNonoverlappingSegments(const std::vector<int>& A, const std::vector<int>& B)
{
    int res = 0;

    if (!A.empty())
    {
        for (int i = 0, r = -1; i < static_cast<int>(A.size()); ++i)
        {
            if (A[i] > r)
            {
                r = B[i];
                ++res;
            }
        }
    }

    return res;
}

int TieRopes(int K, const std::vector<int>& A)
{
    int sum = 0, count = 0;

    for (int l : A)
    {
        sum += l;

        if (sum >= K)
        {
            sum = 0;
            ++count;
        }
    }

    return count;
}

int MinAbsSum(std::vector<int>& A)
{
    /*if (A.empty())
    {
        return 0;
    }

    std::unordered_set<int> allSums;
    allSums.insert(A[0]);

    std::for_each(++A.begin(), A.end(), [&](int a)
        {
            std::unordered_set<int> sums;

            for (int s : allSums)
            {
                sums.insert(s - a);
                sums.insert(s + a);
            }

            allSums = sums;
        });

    int res = std::numeric_limits<int>::max();

    for (int s : allSums)
    {
        res = std::min(res, std::abs(s));
    }

    return res;*/

    ///////////////////////////////////////////////////////////////////////////////////////

    /*int maxSum = 0;

    std::for_each(A.begin(), A.end(), [&maxSum](int& a) { maxSum += (a = std::abs(a)); });

    std::vector<bool> sums(maxSum + 1, false);
    sums[0] = true;

    for (auto a : A)
    {
        for (int s = maxSum; s >= 0; --s)
        {
            if (sums[s] && s + a <= maxSum)
            {
                sums[s + a] = true;
            }
        }
    }

    int res = maxSum;

    for (int s = 0; s <= maxSum / 2; ++s)
    {
        if (sums[s])
        {
            res = std::min(res, maxSum - s * 2);
        }
    }

    return res;*/

    ///////////////////////////////////////////////////////////////////////////////////

    std::set<int, std::greater<>> nums;
    std::unordered_map<int, int> counts;
    int maxSum = 0;

    for (int a : A)
    {
        if (a != 0)
        {
            nums.insert(std::abs(a));
            ++counts[std::abs(a)];
            maxSum += std::abs(a);
        }
    }

    std::vector<int> sums(maxSum + 1, -1);
    sums[0] = 0;

    for (int n : nums)
    {
        for (int s = 0; s < maxSum; ++s)
        {
            if (sums[s] >= 0)
            {
                sums[s] = counts[n];
            }
            else if (s >= n && sums[s - n] > 0)
            {
                sums[s] = sums[s - n] - 1;
            }
        }
    }

    int res = maxSum;

    for (int s = 0; s <= maxSum / 2; ++s)
    {
        if (sums[s] >= 0)
        {
            res = std::min(res, maxSum - 2 * s);
        }
    }

    return res;
}

int NumberSolitaire(const std::vector<int>& A)
{
    std::vector<int> sum(A.size(), std::numeric_limits<int>::min());
    sum[0] = A[0];

    for (size_t i = 1; i < A.size(); ++i)
    {
        for (size_t dice = 1; dice <= std::min(i, static_cast<size_t>(6)); ++dice)
        {
            sum[i] = std::max(sum[i], A[i] + sum[i - dice]);
        }
    }

    return sum[A.size() - 1];
}


int main()
{
    //std::cout << BinaryGap(1041);

    //std::vector<int> v{ 1,2,3 };
    //for (auto i : CyclicRotation(v, 4)) { std::cout << i << " "; }

    //std::cout << OddOccurrencesInArray({ 1,2,2,1,3,4,3 });

    //std::cout << PermMissingElem({ 1,5,2,3,6 });

    //std::cout << TapeEquilibrium({ 3,1,2,4,3 });

    //std::cout << FrogRiverOne(5, { 1,3,1,4,2,3,5,4 });

    //for (auto i : MaxCounters(5, { 3,4,4,6,1,4,4 })) { std::cout << i << " "; }

    //std::cout << MissingInteger({ 1,3,5,6,1,2 });

    //std::cout << PermCheck({ 4,1,3,2 });

    //std::cout << CountDiv(1, 6, 2);

    //for (auto i : GenomicRangeQuery("CAGCCTA", { 2,5,0 }, { 4,5,6 })) { std::cout << i << " "; }

    //std::cout << MinAvgTwoSlice({ 4,2,2,5,1,5,8 });

    //std::cout << PassingCars({ 0,1,0,1,1 });

    //std::cout << Distinct({ 2,1,1,2,3,1 });

    //std::cout << MaxProductOfThree({ -3,1,2,-2,5,6 });

    //std::cout << NumberOfDiscIntersections({ 1,5,2,1,4,0 } /*{1, 2147483647, 0}*/);

    //std::cout << Triangle(/*{ 10,50,5,1 }*/{ 10,2,5,1,8,20 });

    //std::cout << Brackets("{[()()]}") << " " << Brackets("([)()]");

    //std::cout << Fish({4,3,2,1,5}, {1,1,0,1,0});

    //std::cout << Nesting("(()(())())") << " " << Nesting("())");

    //std::cout << StoneWall({ 8,8,5,7,9,8,7,4,8 });

    //std::cout << Dominator({ 3,4,3,2,3,-1,3,3 });

    //std::cout << EquiLeader({ 4,3,4,4,4,2 });

    //std::cout << MaxSliceSum({ 3,2,-6,4,0 });

    //std::cout << MaxProfit({ 23171,21011,21123,21366,21013,21367 });

    //std::cout << MaxDoubleSliceSum({3,2,6,-1,4,5,-1,2}/*{ -1,-1,-1,-1,-1 }*/);

    //std::cout << CountFactors(24);

    //std::cout << MinPerimeterRectangle(30);

    //std::cout << Flags({ 1,5,3,4,3,4,1,2,3,4,6,2 });

    //std::cout << Peaks({1,2,3,4,3,4,1,2,3,4,6,2});

    //for (auto i : CountNonDivisible({ 3,1,2,3,6 })) { std::cout << i << " "; }

    //for (int i : CountSemiprimes(26, { 1,4,16 }, { 26,10,20 })) { std::cout << i << " "; }

    //std::cout << GreatestCommonDivider(12, 12);

    //std::cout << ChocolatesByNumbers(10, 4);

    //std::cout << CommonPrimeDivisors({15,10,3}, {75,30,5});

    //std::cout << FibonacciAt(11);

    //for (int i : FibonacciUpTo(187)) { std::cout << i << " "; }

    //std::vector<int> v{ 0,0,0,1,1,0,1,0,0,0,0 };
    //std::cout << FibFrog(v);

    //for (int i : Ladder( { 4,4,5,5,1 }, { 3,2,4,3,1 })) { std::cout << i << " "; }

    //std::cout << MinMaxDivision(3, 5, { 2,1,5,1,2,2,2 });

    //std::vector<int> A{ 1, 4, 5, 8 }, B{ 4, 5, 9, 10 };
    //std::cout << NailingPlanks(A, B, { 4, 6, 7, 10, 2 });

    //std::cout << AbsDistinct({ -5,-3,-1,0,3,6 });

    //std::cout << CountDistinctSlices(6, { 3,4,5,5,2 });

    //std::vector<int> A{ 10,2,5,1,8,12 };
    //std::cout << CountTriangles(A);

    //std::vector<int> A{ 1, 4, -3};
    //std::cout << MinAbsSumOfTwo(A);

    //std::cout << MaxNonoverlappingSegments({ 1,3,7,9,9 }, { 5,6,8,9,10 });

    //std::cout << TieRopes(4, { 1,2,3,4,1,1,3 });

    //std::vector<int> A{ 3,1 };// { 3, 3, 3, 4, 5 };//{ 1, 5, 2, -2 };
    //std::cout << MinAbsSum(A);

    //std::cout << NumberSolitaire({ 1, -2, 4, 3, -1, -3, -7, 4, -9 });
}
