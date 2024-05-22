#include "delimeter.hpp"
#include <cctype>
#include <stdexcept>
#include <iostream>
std::istream& ravinskij::operator>>(std::istream& in, CharDelimeter&& exp)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  char c = 0;
  in >> c;
  if (std::tolower(c) != exp.expected)
  {
    in.setstate(std::ios::failbit);
    std::cout << c << '\n';
    throw std::logic_error("Fail delimeter");
  }
  return in;
}