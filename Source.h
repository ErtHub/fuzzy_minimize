#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>



struct TextPos
{
    int lineNumber;
    int charNumber;
    explicit TextPos(int l = 0, int c = 0): lineNumber(l), charNumber(c) {}
};

class Source
{
    bool good = false;
    int etotal, einline;

    const std::string fileName;
    std::ifstream istr;
    std::string line;
    TextPos pos;

    void printLine()
    {
        std::cout << std::setw(12) << pos.lineNumber << ' ' << line;
    }

    bool nextLine();

public:
    explicit Source(const std::string& fname);
    ~Source();
    void error(int errcode, const TextPos &tp, const std::string &expl = "");
    char nextChar();
    const TextPos& getPos() const
    {
        return pos;
    }
    bool isGood() const
    {
        return good;
    }
};
#endif
