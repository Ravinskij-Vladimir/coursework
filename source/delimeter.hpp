#ifndef DELIMETER_HPP
#define DELIMETER_HPP
#include <istream>

namespace ravinskij
{
  struct CharDelimeter
  {
    char expected;
  };
  std::istream& operator>>(std::istream& in, CharDelimeter&& exp);
}
#endif