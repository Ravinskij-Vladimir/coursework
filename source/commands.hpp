#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <set>
#include <map>
#include <vector>
namespace ravinskij
{
    void printHelp();

    using fileSet = std::set<std::string>;
    void addText(std::istream&, fileSet&);
    void saveText(std::istream&, fileSet&);
    void deleteText(std::istream&, fileSet&);
    void printText(std::istream&, std::ostream&, const fileSet&);

    using encodeMap = std::map<char, std::vector<bool>>;
    using encodesTable = std::map<std::string, encodeMap>;
    void createEncoding(std::istream&, encodesTable&);
    void deleteEncoding(std::istream&, encodesTable&);
    void encode(std::istream&, encodesTable&, fileSet&);
    void decode(std::istream&, encodesTable&, fileSet&);
    void addEncoding(std::istream&, encodesTable&);
    void saveEncoding(std::istream&, encodesTable&);

    void compareEncodings(std::istream&, const encodesTable&);
}

#endif