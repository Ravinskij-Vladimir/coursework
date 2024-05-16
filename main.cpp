#include <iostream>
#include <fstream>
#include <limits>
#include <string>

int main()
{
  std::string fileName;
  std::cin >> fileName;
  std::ifstream in(fileName);
  if (in.is_open())
  {
    while (!in.eof())
    {
      std::string buf;
      std::getline(in, buf, '\n');
      std::cout << buf << '\n';
    }
  }
  else 
  {
    std::cerr << "No file found\n";
    return 1;
  }
  return 0;
}