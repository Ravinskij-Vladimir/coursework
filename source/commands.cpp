#include "commands.hpp"
#include <iostream>

namespace rav = ravinskij;

void rav::printHelp()
{
  std::cout << "Usage: huffman [COMMAND] [FILES]...\n";
  std::cout << "Working with encoding and decoding texts from files.\n";
  std::cout << "Example: huffman encode text1 text2 encoding1\n";
  std::cout << "\nWorking with text:\n";
  std::cout << "\tadd-text\t <text-name> <file>\n"; //\t add text to work with
  std::cout << "\tsave-text\t <text-name> <file>\n"; //\t write text to the file (content of the file will be overwritten)
  std::cout << "\tdelete-text\t <text-name>\n"; // \t\t delete text to work with
  std::cout << "\tprint-text\t <text-name>\n"; // \t\t print text to the console

  std::cout << "\nEncoding/decoding:\n";
  std::cout << "\tcreate-encoding\t <text-name> <encoding-name>\t\n";
  std::cout << "\tdelete-encoding\t <encoding-name>\t\n";
  std::cout << "\tdecode\t\t <encoded-text> <decoded-text> <encoding-name>\t\n";
  std::cout << "\tadd-encoding\t <encoding-name> <file>\t\n";
  std::cout << "\tsave-encoding\t <encoding-name> <file>\t\n";

  std::cout << "\nComparing encodings:\n";
  std::cout << "\tcompare-encodings <text-name> <encoding-name-1> <encoding-name-2> <...> <encoding-name-N>\t";
  std::cout << "\n\nDescrption:\n";
  std::cout << "\tadd-text\t <text-name> <file> \t add text to work with\n";
}

void rav::addText(std::istream& in, fileSet& files)
{
}
void rav::saveText(std::istream& in, fileSet& files)
{
}
void rav::deleteText(std::istream& in, fileSet& files)
{
}
void rav::printText(std::istream& in, std::ostream& out, const fileSet& files)
{
}

void rav::createEncoding(std::istream& in, encodesTable& encodings)
{
}
void rav::deleteEncoding(std::istream& in, encodesTable& encodings)
{
}
void rav::encode(std::istream& in, encodesTable& encodings, fileSet& files)
{
}
void rav::decode(std::istream& in, encodesTable& encodings, fileSet& files)
{
}
void rav::addEncoding(std::istream& in, encodesTable& encodings)
{
}
void rav::saveEncoding(std::istream& in, encodesTable& encodings)
{
}

void rav::compareEncodings(std::istream& in, const encodesTable& encodings)
{
}
