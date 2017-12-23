#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>



struct TextPos
{
    int ln;
    int cn;
    explicit TextPos(int l = 0, int c = 0): ln(l), cn(c) {}
};

class Source
{
    bool good = false;
    int etotal, einline;

    const std::string fn;
    std::ifstream istr;
    std::string line;
    TextPos tpos;

    void prntLine()
    {
        std::cout << std::setw(5) << tpos.ln << ' ' << line;
    }

    bool nextLine();

public:
    explicit Source(const std::string& file);
    ~Source();
    void error(int ec, const TextPos &tp, const std::string &mt = "", const std::string &at = "");
    char nextChar();
    const TextPos& getPos() const
    {
        return tpos;
    }
    bool isGood() const
    {
        return good;
    }
};
#endif
