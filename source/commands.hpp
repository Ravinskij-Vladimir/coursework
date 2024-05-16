#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <vector>
namespace ravinskij
{
    void printHelp();

    using fileSet = std::set<std::string>;
    void addText(const std::string&, std::ifstream&, fileSet&);
    void saveText(const std::string&, std::ofstream&, fileSet&);
    void deleteText(const std::string&, fileSet&);
    void printText(const std::string&, const fileSet&);

    using encodeMap = std::map<char, std::vector<bool>>;
    using encodesTable = std::map<std::string, encodeMap>;
    void createEncoding(const std::string&, const std::string&, encodesTable&);
    void deleteEncoding(const std::string&, encodesTable&);
    void encode(const std::string&, const std::string&, const std::string&, encodesTable&, fileSet&);
    void decode(const std::string&, const std::string&, const std::string&, encodesTable&, fileSet&);
    void addEncoding(const std::string&, std::ifstream&, encodesTable&);
    void saveEncoding(const std::string&, std::ofstream&, encodesTable&);

    void compareEncodings(const std::string&, const std::list<std::string>&, const encodesTable&);
}

#endif