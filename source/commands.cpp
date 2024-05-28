#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iomanip>
#include "codeWrappers.hpp"
#include "scopeGuard.hpp"

namespace rav = ravinskij;

void rav::printHelp(std::ostream& out)
{
  out << "Usage: huffman [COMMAND] [FILES]...\n";
  out << "Working with encoding and decoding texts from files.\n";
  out << "Example: huffman encode text1 text2 encoding1\n";
  out << "\nWorking with text:\n";
  out << "\tadd-text\t <text-name> <file>\n";
  out << "\tsave-text\t <text-name> <file>\n";
  out << "\tdelete-text\t <text-name>\n";
  out << "\tprint-text\t <text-name>\n";

  out << "\nEncoding/decoding:\n";
  out << "\tcreate-encoding\t <text-name> <encoding-name>\t\n";
  out << "\tdelete-encoding\t <encoding-name>\t\n";
  out << "\tencode\t\t <text-name>  <encoded-name> <encoding-name> \t\n";
  out << "\tdecode\t\t <encoded-text> <decoded-text> <encoding-name>\t\n";
  out << "\tadd-encoding\t <encoding-name> <file>\t\n";
  out << "\tsave-encoding\t <encoding-name> <file>\t\n";

  out << "\nComparing encodings:\n";
  out << "\tcompare-encodings <text-name> <encoding-name-1> <encoding-name-2> <...> <encoding-name-N>\t";

  out << "\n\nDescription:\n";
  out << "\tadd-text\t add text to work with\n";
  out << "\tsave-text\t write text to the file (content of the file will be overwritten)\n";
  out << "\tdelete-text\t delete text to work with\n";
  out << "\tprint-text\t print the text to the console\n";
  out << "\tcreate-encoding\t create encoding table in accordance with the text\n";
  out << "\tdelete-encoding\t delete encoding table to work with\n";
  out << "\tencode\t\t encode the text in accordance with the encoding table\n";
  out << "\tdecode\t\t decode the text in accordance with the encoding table\n";
  out << "\tadd-encoding\t add the encoding table from the file in the format 'symbol - binary code'\n";
  out << "\tsave-encoding\t save the encoding table to the file in the format 'symbol - binary code'\n";
  out << "\tcompare-encodings\t Compares encodings applied to the same text.\n";
  out << "\t\t\t\tDisplays a comparison table of the format on the console:";
  out << "encoding name - size of the compressed text in bytes - \n\t\t\t\tcompression percentage accurate to two decimal places";
  out << "(for the source text, the text name is displayed instead of the encoding).\n";
}

// void copyFile(std::ifstream& in, std::ostream& out)
// {
//   while(!in.eof())
//   {
//     std::string line;
//     std::getline(in, line, '\n');
//     out << line << '\n';
//   }
// }

constexpr int bitsInByte()
{
  return 8;
}

void readAlphabet(std::istream &input, std::map< char, size_t > &alphabet)
{
  char c = 0;
  rav::ScopeGuard guard(input);
  input >> std::noskipws;
  while (!input.eof())
  {
    input >> c;
    alphabet[c]++;
  }
}

auto getNodePtr(const std::pair< char, size_t >& map)
{
  return std::make_shared< rav::Node >(map.second, map.first);
}

void buildHuffmanTree(std::list< rav::nodePtr > &lst, const std::map< char, size_t > &alphabet, rav::NodeComparator comp)
{
  // for (auto itr = alphabet.cbegin(); itr != alphabet.cend(); ++itr)
  // {
  //   rav::nodePtr p = std::make_shared< rav::Node >(itr->second, itr->first);
  //   // p->symbol = itr->first;
  //   // p->frequency = itr->second;
  //   lst.push_back(p);
  // }
  std::transform(alphabet.cbegin(), alphabet.cend(), std::back_inserter(lst), getNodePtr);

  while (lst.size() != 1)
  {
    lst.sort(comp);

    rav::nodePtr leftChild = lst.front();
    lst.pop_front();
    rav:: nodePtr rightChild = lst.front();
    lst.pop_front();

    rav::nodePtr parent = std::make_shared< rav::Node >(leftChild, rightChild);
    lst.push_back(parent);
  }
}

void buildTable(const rav::nodePtr& root, std::vector< bool > &code, rav::encodeMap &table)
{
  if (root->left != nullptr)
  {
    code.push_back(0);
    buildTable(root->left, code, table);
  }

  if (root->right != nullptr)
  {
    code.push_back(1);
    buildTable(root->right, code, table);
  }

  if (root->left == nullptr && root->right == nullptr)
    table[root->symbol] = code;

  code.pop_back();
}

void encodeImpl(const rav::encodeMap &table, std::istream &input, std::ostream &output)
{
  int position = 0;
  char buf = 0;
  rav::ScopeGuard guard(input);
  input >> std::noskipws;
  char c = 0;
  while (!input.eof())
  {
    input >> c;
    std::vector< bool > temp = table.at(c);
    for (size_t i = 0; i < temp.size(); i++)
    {
      buf = buf | temp[i] << (bitsInByte() - 1 - position);
      position++;
      if (position == bitsInByte())
      {
        position = 0;
        output << buf;
        buf = 0;
      }
    }
  }
}

void decodeImpl(const std::list< rav::nodePtr >& travers, std::istream &input, std::ostream &output)
{
  rav::nodePtr root = travers.front();
  rav::nodePtr traverser = root;
  int position = 0;
  char byte = 0;
  byte = input.get();
  while (!input.eof())
  {
    bool checkedBitState = byte & (1 << (bitsInByte() - 1 - position));
    if (checkedBitState)
      traverser = traverser->right;
    else
      traverser = traverser->left;
    if (traverser->left == nullptr && traverser->right == nullptr)
    {
      if (traverser->symbol != EOF)
      {
        output << traverser->symbol;
        traverser = root;
      }
    }
    position++;
    if (position == bitsInByte())
    {
      position = 0;
      byte = input.get();
    }
  }
}

void rav::addText(std::istream& in, fileTable& files)
{
  std::string textName, fileName;
  in >> textName >> fileName;

  if (files.find(textName) != files.cend())
  {
    throw std::logic_error("Such text already exists");
  }

  std::ifstream input(fileName);
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  files.insert({textName, fileName});
}

void rav::saveText(std::istream& in, fileTable& files)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  if (files.find(textName) == files.cend())
  {
    throw std::logic_error("Requested text is not found");
  }
  std::ifstream input(files.find(textName)->second);
  std::ofstream output(fileName);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  in >> std::noskipws;
  using input_it_t = std::istreambuf_iterator< char >;
  using output_it_t = std::ostream_iterator< char >;
  std::copy(input_it_t{ input }, input_it_t{}, output_it_t{ output, "" });
  //copyFile(input, output);
}

void rav::deleteText(std::istream& in, fileTable& files)
{
  std::string textName;
  in >> textName;
  if (files.find(textName) == files.cend())
  {
    throw std::logic_error("Requested text is not found");
  }
  files.erase(textName);
}

void rav::printText(std::istream& in, std::ostream& out, const fileTable& files)
{
  std::string textName;
  in >> textName;
  if (files.find(textName) == files.cend())
  {
    throw std::logic_error("Requested text is not found");
  }
  std::ifstream input(files.find(textName)->second);
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  using input_it_t = std::istreambuf_iterator< char >;
  using output_it_t = std::ostream_iterator< char >;
  std::copy(input_it_t{ input }, input_it_t{}, output_it_t{ out, "" });
  out << '\n';
  //copyFile(input, out);
}

void rav::createEncoding(std::istream& in, encodesTable& encodings, traverserTable& traverses, const fileTable& files)
{
  std::string textName, encodingName;
  in >> textName >> encodingName;
  if (encodings.find(encodingName) != encodings.cend())
  {
    throw std::logic_error("Such encoding already exists");
  }

  std::ifstream input(files.at(textName));
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }

  std::map< char, size_t > alphabet;
  readAlphabet(input, alphabet);
  std::list< rav::nodePtr > tree;
  buildHuffmanTree(tree, alphabet, rav::NodeComparator());
  traverses.insert({encodingName, tree});
  rav::nodePtr root = tree.front();
  std::vector< bool > code;
  buildTable(root, code, encodings[encodingName]);
}

void rav::deleteEncoding(std::istream& in, encodesTable& encodings, traverserTable& traverses)
{
  std::string encodingName;
  in >> encodingName;
  if (encodings.find(encodingName) == encodings.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  encodings.erase(encodingName);
  traverses.erase(encodingName);
}

void rav::encode(std::istream& in, const encodesTable& encodings, fileTable& files)
{
  std::string textName, encodedName, encodingName;
  in >> textName >> encodedName >> encodingName;

  if (encodings.find(encodingName) == encodings.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  if (files.find(textName) == files.cend())
  {
    throw std::logic_error("No such text is provided");
  }
  if (textName == encodedName)
  {
    throw std::logic_error("Names collision occured");
  }

  std::ifstream input(files[textName]);
  files.insert({encodedName, encodedName});
  std::ofstream output(files[encodedName], std::ios::out | std::ios::binary);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open files");
  }
  encodeImpl(encodings.find(encodingName)->second, input, output);
}

void rav::decode(std::istream& in, const traverserTable& traverses, fileTable& files)
{
  std::string textName, decodedName, encodingName;
  in >> textName >> decodedName >> encodingName;
  if (textName == decodedName)
  {
    throw std::logic_error("Names collision has occured");
  }
  if (traverses.find(encodingName) == traverses.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  std::ifstream input(files.at(textName), std::ios::in | std::ios::binary);
  std::ofstream output(decodedName);
  if (!input.is_open() || !output.is_open())
  {
    throw std::logic_error("Couldn't open any file");
  }
  std::list< rav::nodePtr > traverser = traverses.find(encodingName)->second;
  decodeImpl(traverser, input, output);
  files.insert({decodedName, decodedName});
}

void rav::addEncoding(std::istream& in, encodesTable& encodings, traverserTable& traverses)
{
  std::string encodingName, fileName;
  in >> encodingName >> fileName;
  if (encodings.find(encodingName) != encodings.cend())
  {
    throw std::logic_error("Such encoding already exists");
  }
  std::ifstream input(fileName);
  if (!input.is_open())
  {
    throw std::logic_error("Couldn't open file");
  }
  rav::encodeMap map;
  std::map< char, size_t > alphabet;
  while (!input.eof())
  {
    char ch = 0;
    std::vector< bool > code;
    size_t freq = 0;
    input >> rav::ReadWrapper{ch, code, freq};
    map.insert({ch, code});
    alphabet.insert({ch, freq});
  }
  encodings.insert({encodingName, map});
  std::list< rav::nodePtr > newTraverser;
  buildHuffmanTree(newTraverser, alphabet, rav::NodeComparator{});
  traverses.insert({encodingName, newTraverser});
}

size_t getFrequency(rav::nodePtr root, const std::pair< char,  std::vector< bool > >& map)
{
  const auto& code = map.second;
  //auto unaryOperator = 
  for (auto it: code)
  {
    if (root == nullptr)
    {
      return 0;
    }
    root = (it == 0 ? root->left : root->right);
  }
  return root->frequency;
}

void rav::saveEncoding(std::istream& in, const encodesTable& encodings, const traverserTable& traverses)
{
  std::string encodingName, fileName;
  in >> encodingName >> fileName;
  auto currEncoding = encodings.find(encodingName);
  if (currEncoding == encodings.cend())
  {
    throw std::logic_error("No such encoding is provided");
  }
  std::ofstream output(fileName);
  auto beginIt = currEncoding->second.cbegin();
  auto endIt = currEncoding->second.cend();
  std::list< size_t > frequencies;
  auto root = traverses.find(encodingName)->second.front();
  auto unaryOperator = std::bind(getFrequency, root, std::placeholders::_1);
  std::transform(beginIt, endIt, std::back_inserter(frequencies), unaryOperator);
  // for (auto it = beginIt; it != endIt; ++it)
  // {
  //   auto traverser = root;
  //   frequencies.push_back(getFrequency(traverser, it->second));
  // }

  output << WriteWrapper{beginIt->first, beginIt->second, frequencies.front()};
  frequencies.pop_front();
  ++beginIt;
  for (auto it = beginIt; it != endIt; ++it)
  {
    output << '\n' << WriteWrapper{it->first, it->second, frequencies.front()};
    frequencies.pop_front();
  }
}

std::ifstream::pos_type getFileSize(const std::string filename)
{
  std::ifstream in(filename, std::ios::ate | std::ios::binary);
  return in.tellg();
}

double getCompessionPercentage(size_t oldSize, size_t newSize)
{
  return static_cast<double>((oldSize - newSize)) / oldSize;
}

void inputArgs(std::list< std::string >& args, std::istream& in)
{
  std::string arg;
  rav::ScopeGuard guard(in);
  in >> std::noskipws;
  char delim = 0;
  in >> delim;
  while (in && delim != '\n')
  {
    in >> arg >> delim;
    args.push_back(arg);
  }
}

void rav::compareEncodings(std::istream& in, std::ostream& output, const fileTable& files, const encodesTable& encodings)
{
  // std::list< std::string > args;
  // std::string arg;
  // rav::ScopeGuard guard(in);
  // in >> std::noskipws;
  // char delim = 0;
  // in >> delim;
  // while (in && delim != '\n')
  // {
  //   in >> arg >> delim;
  //   if (in)
  //   {
  //     args.push_back(arg);
  //   }
  // }
  std::list< std::string > args;
  inputArgs(args, in);
  if (args.empty())
  {
    throw std::logic_error("No arguments are provided");
  }
  // for (const auto& arg: args)
  // {
  //   out << arg << ' ';
  // }
  // out << '\n';
  std::string fileName = args.front();
  args.pop_front();
  if (files.find(fileName) == files.cend())
  {
    throw std::logic_error("No such file is provided");
  }
  //output << getFileSize(files.find(fileName)->second) << '\n';
  std::ifstream file(files.find(fileName)->second);
  ScopeGuard outGuard(output);

  output << std::fixed << std::setprecision(2);
  size_t fileSize = getFileSize(files.find(fileName)->second);
  output << fileName << ' ' << fileSize << ' ' << getCompessionPercentage(fileSize, fileSize) << '\n';
  file.close();
  for (const auto& arg: args)
  {
    if (encodings.find(arg) == encodings.cend())
    {
      throw std::logic_error("No such encoding is provided");
    }
    file.open(files.find(fileName)->second);
    std::ofstream out(arg, std::ios::binary);
    encodeImpl(encodings.find(arg)->second, file, out);
    out.close();
    file.close();
    size_t compressedSize = getFileSize(arg);
    output << arg << ' ' << compressedSize << ' ' << getCompessionPercentage(fileSize, compressedSize) << '\n';
  }
}

std::string getFirst(const std::pair< std::string, std::string >& textPair)
{
  return textPair.first;
}

std::string getSecond(const std::pair< std::string, std::string >& textPair)
{
  return textPair.second;
}

std::string getAll(const std::pair< std::string, std::string >& textPair)
{
  return textPair.first + " " + textPair.second;
}

void rav::printFiles(std::istream&, std::ostream& out, const fileTable& files)
{
  if (files.empty())
  {
    out << "<EMPTY_TEXT>\n";
    return;
  }
  // auto beginIt = files.cbegin();
  // out << beginIt->second;
  // ++beginIt;
  // for (auto it = beginIt; it != files.cend(); ++it)
  // {
  //   out << ' ' << it->second;
  // }
  using output_it_t = std::ostream_iterator< std::string >;
  std::transform(files.cbegin(), files.cend(), output_it_t{ out, " " }, getSecond);
  out << '\n';
}

void rav::printTexts(std::istream&, std::ostream& out, const fileTable& files)
{
  if (files.empty())
  {
    out << "<EMPTY_TEXT>\n";
    return;
  }
  // auto beginIt = files.cbegin();
  // out << beginIt->first;
  // ++beginIt;
  // for (auto it = files.cbegin(); it != files.cend(); ++it)
  // {
  //   out << ' ' << it->first;
  // }
  using output_it_t = std::ostream_iterator< std::string >;
  std::transform(files.cbegin(), files.cend(), output_it_t{ out, " " }, getFirst);
  out << '\n';
}

void rav::printAll(std::istream&, std::ostream& out, const fileTable& files)
{
  if (files.empty())
  {
    out << "<EMPTY_TEXT>\n";
    return;
  }
  // for (auto it = files.cbegin(); it != files.cend(); ++it)
  // {
  //   out << it->first << ' ' << it->second << '\n';
  // }
  using output_it_t = std::ostream_iterator< std::string >;
  std::transform(files.cbegin(), files.cend(), output_it_t{ out, " " }, getAll);
  out << '\n';
}