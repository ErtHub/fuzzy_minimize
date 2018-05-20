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
    explicit TextPos(int l = 1, int c = 1): lineNumber(l), columnNumber(c) {}
};

class Reader
{
    bool good = false;
    int errcount;

    const std::string fileName;
    std::ifstream istr;
    TextPos pos;

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
