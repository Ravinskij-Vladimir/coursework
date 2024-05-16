#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iomanip>
#include "scopeGuard.hpp"

constexpr int bitsInByte()
{
	return 8;
}

struct Node
{
	int frequency;
	char symbol;
	Node *left, *right;

	Node() : frequency(0),
			 symbol(0),
			 left(nullptr),
			 right(nullptr)
	{
	}

	Node(Node *leftNode, Node *rightNode) : frequency(0),
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

std::vector<bool> code;
std::map<char, std::vector<bool>> table;

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
	ravinskij::ScopeGuard guard(input);
	input >> std::noskipws;
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

int main(int argc, char *argv[])
{
	////// считаем частоты символов
	std::ifstream input("/home/denny/Рабочий стол/coursework/1.txt", std::ios::out | std::ios::binary);

	std::map<char, int> m;
	readAlphabet(input, m);

	////// записываем начальные узлы в список std::list

	std::list<Node *> t;
	buildHuffmanTree(t, m, NodeComparator());

	Node *root = t.front(); // root - указатель на вершину дерева

	////// создаем пары 'символ-код':

	buildTable(root, code, table);

	////// Выводим коды в файл output.txt

	std::ofstream output("/home/denny/Рабочий стол/coursework/output.txt", std::ios::out | std::ios::binary);

	int position = 0;
	char buf = 0;
	while (!input.eof())
	{
		char c = input.get();
		std::vector<bool> x = table[c];
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

	input.close();
	output.close();

	///// считывание из файла output.txt и преобразование обратно

	std::ifstream in("/home/denny/Рабочий стол/coursework/output.txt", std::ios::in | std::ios::binary);

	setlocale(LC_ALL, "Russian"); // чтоб русские символы отображались в командной строке

	Node *p = root;
	position = 0;
	char byte;
	byte = in.get();
	while (!in.eof())
	{
		bool b = byte & (1 << (bitsInByte() - 1 - position));
		if (b)
			p = p->right;
		else
			p = p->left;
		if (p->left == nullptr && p->right == nullptr)
		{
			std::cout << p->symbol;
			p = root;
		}
		position++;
		if (position == bitsInByte())
		{
			position = 0;
			byte = in.get();
		}
	}
	std::cout << '\n';
	in.close();

	return 0;
}
