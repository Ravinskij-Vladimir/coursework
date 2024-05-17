#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <map>
#include <map>
#include <vector>
namespace ravinskij
{
    void printHelp();

    using fileTable = std::multimap<std::string, std::string>;
    void addText(std::istream&, fileTable&);
    void saveText(std::istream&, fileTable&);
    void deleteText(std::istream&, fileTable&);
    void printText(std::istream&, std::ostream&, const fileTable&);

    using encodeMap = std::map<char, std::vector<bool>>;
    using encodesTable = std::map<std::string, encodeMap>;
    void createEncoding(std::istream&, encodesTable&);
    void deleteEncoding(std::istream&, encodesTable&);
    void encode(std::istream&, encodesTable&, fileTable&);
    void decode(std::istream&, encodesTable&, fileTable&);
    void addEncoding(std::istream&, encodesTable&);
    void saveEncoding(std::istream&, encodesTable&);

    void compareEncodings(std::istream&, const encodesTable&);
}

#endif