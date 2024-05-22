#include "codeWrappers.hpp"
#include <iostream>
#include "scopeGuard.hpp"
#include "delimeter.hpp"

namespace rav = ravinskij;
std::ostream& rav::operator<<(std::ostream& out, rav::WriteWrapper&& wrapper)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    
    out << '"' << wrapper.symbol << '"' << '-';
    for (bool bit: wrapper.code)
    {
        out << bit;
    }
    return out;
}

std::istream& rav::operator>>(std::istream& in, rav::ReadWrapper&& wrapper)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    using del = CharDelimeter;
    in >> std::noskipws;
    in >> del{'"'} >> wrapper.symbol >> del{'"'} >> del{'-'};
    if (!in)
    {
        return in;
    }
    char bit = 0;
    while (in && bit != '\n')
    {
        in >> bit;
        wrapper.code.push_back(static_cast<bool>(bit - '0'));
    }
    wrapper.code.pop_back();
    in >> std::skipws;
    return in;
}