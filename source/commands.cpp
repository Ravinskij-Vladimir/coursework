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

void rav::addText(const std::string& text, std::ifstream& file, fileSet& files)
{
}

void rav::saveText(const std::string& text, std::ofstream& file, fileSet& files)
{
}
void rav::deleteText(const std::string& text, fileSet& files)
{
}
void rav::printText(const std::string& text, const fileSet& files)
{
}

void rav::createEncoding(const std::string& encoding, const std::string&encode, encodesTable&encodings)
{
}
void rav::deleteEncoding(const std::string& encoding, encodesTable& encodings)
{
}
void rav::encode(const std::string& text, const std::string& encoded, const std::string& encoding, encodesTable& encodings, fileSet& files)
{
}
void rav::decode(const std::string& text, const std::string& encoded, const std::string& encoding, encodesTable& encodings, fileSet& files)
{
}
void rav::addEncoding(const std::string& encoding, std::ifstream& file, encodesTable& encodings)
{
}
void rav::saveEncoding(const std::string& encoding, std::ofstream& file, encodesTable& encodings)
{
}

void rav::compareEncodings(const std::string& text, const std::list<std::string>& encodesList, const encodesTable& encodings)
{
}
