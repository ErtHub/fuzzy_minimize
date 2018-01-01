#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>



struct TextPos
{
    int lineNumber;
    int columnNumber;
    explicit TextPos(int l = 0, int c = 0): lineNumber(l), columnNumber(c) {}
};

class Reader
{
    bool good = false;
    int errcount;
    bool firstErrorInLine;

    const std::string fileName;
    std::ifstream istr;
    std::string line;
    TextPos pos;

    bool nextLine();

public:
    explicit Reader(const std::string& fname);
    ~Reader();
    void alert(int errcode, const TextPos& tp, const std::string& expl = "", const std::string& what = "Unknown error");
    char nextChar();
    const TextPos& getPos() const
    {
        return pos;
    }
    bool isGood() const
    {
        return good;
    }
    int getErrcount() const
    {
        return errcount;
    }
};
#endif
