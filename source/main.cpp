#include "commands.hpp"
#include <functional>
#include <iostream>
#include <unordered_map>
#include <string>
#include <limits>

int main()
{
    using namespace ravinskij;
    encodesTable table;
    fileSet files;

    using Command = std::function<void(std::istream &)>;
    std::unordered_map<std::string, Command> commands;
    {
        using namespace std::placeholders;
        commands["add-text"] = std::bind(ravinskij::addText, _1, std::ref(files));
        commands["save-text"] = std::bind(ravinskij::saveText, _1, std::ref(files));
        commands["delete-text"] = std::bind(ravinskij::deleteText, _1, std::ref(files));
        commands["print-text"] = std::bind(ravinskij::printText, _1, std::ref(std::cout), std::cref(files));
        commands["create-encoding"] = std::bind(ravinskij::createEncoding, _1, std::ref(table));
        commands["delete-encoding"] = std::bind(ravinskij::deleteEncoding, _1, std::ref(table));
        commands["encode"] = std::bind(ravinskij::encode, _1, std::ref(table), std::ref(files));
        commands["decode"] = std::bind(ravinskij::decode, _1, std::ref(table), std::ref(files));
        commands["add-encoding"] = std::bind(ravinskij::addEncoding, _1, std::ref(table));
        commands["save-encoding"] = std::bind(ravinskij::saveEncoding, _1, std::ref(table));
        commands["compare-encodings"] = std::bind(ravinskij::compareEncodings, _1, std::cref(table));
    }

    std::string cmd;

    while (std::cin >> cmd)
    {
        try
        {
            commands.at(cmd)(std::cin);
        }
        catch (const std::logic_error &e)
        {
            std::cout << "<INVALID_COMMAND>" << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "<ERROR: " << e.what() << ">\n";
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}