#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <set>

int main()
{
  std::set<std::string> files;
  std::string fileName;
  std::cin >> fileName;
  std::string fileName2 = "3.txt";
  files.insert(fileName);
  files.insert(fileName2);
  if (files.find(fileName) == files.cend())
  {
    std::cerr << "No such file\n";
    return 1;
  }
  std::ifstream in(*files.find(fileName));
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