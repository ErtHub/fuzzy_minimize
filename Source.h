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
    int etotal, einline;

    const std::string fn;
    std::ifstream istr;
    std::string Line;//TODO powiedziec doktorowi, ze sie wola max_size

    TextPos tpos;
//TODO kilka funkcji
    void PrntLine()
    {
        std::cout << std::setw(5) << tpos.ln << ' ' << Line;
    }

    bool NextLine();

public:
    explicit Source(const std::string& file);
    ~Source();
    void Error(int ec, const TextPos& tp, const std::string& mt = "", const std::string& at = "");
    char NextChar();
    const TextPos& GetPos() const
    {
        return tpos;
    }
};
#endif