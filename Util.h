#ifndef UTIL_H
#define UTIL_H

#include <cstdarg>
#include "Scanner.h"

const int EOS = -1;

class TokenTypeSet
{
    enum
    {
        size = MAXSYMB,
        ubits = sizeof(unsigned) * CHAR_BIT,
        nwords = (size - 1) / ubits + 1
    };

    std::vector<unsigned> content;

public:
    TokenTypeSet() : content(std::vector<unsigned>(nwords, 0)) {};
    explicit TokenTypeSet(int e);

    TokenTypeSet(int, int, ...);

    TokenTypeSet operator+(const TokenTypeSet& t) const;
    bool contains(int e)const;
    friend std::ostream& operator<<(std::ostream& os, const TokenTypeSet& t);
};

class Trace
{
    static int callLevel;

public:
    Trace(const std::string& text, const TokenTypeSet& s);
    ~Trace();
    static void prefix();
    static int traceOn;
    static int showSymbols;
};
#endif

