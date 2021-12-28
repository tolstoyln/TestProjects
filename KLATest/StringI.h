#pragma once

#include <string>

namespace _StrI
{
// Make std::string case insensitive inside std containers:
struct char_traits_i : public std::char_traits<char>
{
    static int __cdecl compare(const char* csz1, const char* csz2, size_t n)
    {
        if (csz1 == 0)
            return (csz2 == 0) ? 0 : -1;
        if (csz2 == 0)
            return 1;
        return _strnicmp(csz1, csz2, n);
    }
};
typedef std::basic_string<char, char_traits_i> string_i;
};
