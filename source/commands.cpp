#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <stdexcept>

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

constexpr int bitsInByte()
{
  return 8;
}

struct Node
{
  size_t frequency;
  char symbol;
  Node *left, *right;

  Node() = default;
  Node(const Node &) = default;
  Node(Node &&) = default;
  Node &operator=(const Node &) = default;
  Node &operator=(Node &&) = default;

  Node(Node *leftNode, Node *rightNode): 
    frequency(0),
    symbol(0),
    left(leftNode),
    right(rightNode)
  {
    frequency = leftNode->frequency + rightNode->frequency;
  }
};

struct NodeComparator
{
  bool operator()(const Node *lhs, const Node *rhs) const
  {
    return lhs->frequency < rhs->frequency;
  }
};

void buildTable(Node *root, std::vector<bool> &code, std::map<char, std::vector<bool>> &table)
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

void readAlphabet(std::istream &input, std::map<char, int> &alphabet)
{
  char c = 0;
  while (!input.eof())
  {
    c = input.get();
    alphabet[c]++;
  }

  input.clear();
  input.seekg(0); // перемещаем указатель снова в начало файла
}

void buildHuffmanTree(std::list<Node *> &lst, const std::map<char, int> &alphabet, NodeComparator comp)
{
  for (auto itr = alphabet.cbegin(); itr != alphabet.cend(); ++itr)
  {
    Node *p = new Node;
    p->symbol = itr->first;
    p->frequency = itr->second;
    lst.push_back(p);
  }

  //////  создаем дерево
  while (lst.size() != 1)
  {
    lst.sort(comp);

    Node *leftChild = lst.front();
    lst.pop_front();
    Node *rightChild = lst.front();
    lst.pop_front();

    Node *parent = new Node(leftChild, rightChild);
    lst.push_back(parent);
  }
}

void encodeAndWrite(const std::map<char, std::vector<bool>> &table, std::istream &input, std::ostream &output)
{
  int position = 0;
  char buf = 0;
  while (!input.eof())
  {
    char c = input.get();
    std::vector<bool> x = table.at(c);
    for (size_t n = 0; n < x.size(); n++)
    {
      buf = buf | x[n] << (bitsInByte() - 1 - position);
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

void decodeAndWrite(Node *root, std::istream &input, std::ostream &output)
{
  Node *traverser = root;
  int position = 0;
  char byte;
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
      output << traverser->symbol;
      traverser = root;
    }
    position++;
    if (position == bitsInByte())
    {
      position = 0;
      byte = input.get();
    }
  }
  output << '\n';
}

void copyFile(std::ifstream& in, std::ostream& out)
{
  while(!in.eof())
  {
    std::string line;
    std::getline(in, line, '\n');
    out << line << '\n';
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
  copyFile(input, std::cout);
  files.insert({textName, fileName});
  input.close();
  for (auto it = files.cbegin(); it != files.cend(); it++)
  {
    std::cout << it->first << ' ' << it->second << '\n';
  }
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
  copyFile(input, output);
  input.close();
  output.close();
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
  copyFile(input, out);
}

void rav::createEncoding(std::istream& in, encodesTable& encodings)
{
}
void rav::deleteEncoding(std::istream& in, encodesTable& encodings)
{
}
void rav::encode(std::istream& in, encodesTable& encodings, fileTable& files)
{
}
void rav::decode(std::istream& in, encodesTable& encodings, fileTable& files)
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
