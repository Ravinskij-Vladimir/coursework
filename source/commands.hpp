#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include <fstream>
#include <list>
namespace ravinskij
{
    void printHelp();

    void addText(const std::string&, std::ifstream&);
    void saveText(const std::string&, std::ofstream&);
    void deleteText(const std::string&);
    void printText(const std::string&);

    void createEncoding(const std::string&, const std::string&);
    void deleteEncoding(const std::string&);
    void encode(const std::string&, const std::string&, const std::string&);
    void decode(const std::string&, const std::string&, const std::string&);
    void addEncoding(const std::string&, std::ifstream&);
    void saveEncoding(const std::string&, std::ofstream&);

    void compareEncodings(const std::string&, const std::list<std::string>&);
}

#endif