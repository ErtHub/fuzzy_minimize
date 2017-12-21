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
    std::string line;//TODO powiedziec doktorowi, ze sie wola max_size i ze history robi balagan w algorytmie h, zapytac
                    //czy maja byc czytane puste funkcje, powiedziec, ze artykuly nie sa publikowane, powiedziec, ze agonia
                    //zapytac o zmiane generatora
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
