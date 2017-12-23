#ifndef UTIL_H
#define UTIL_H

#include <cstdarg>
#include "Scanner.h"

const int EOS=-1;

class SymSet
{
    enum
    {
        size = MAXSYM,
        ubits = (sizeof(unsigned) * CHAR_BIT),
        nwords = ((size-1)/ubits+1)
    };

    std::vector<unsigned> s;

public:
    SymSet() : s(std::vector<unsigned>(nwords, 0)) {};
    explicit SymSet(int e);

    SymSet(int, int, ...);

    SymSet operator+(const SymSet& t) const;
    bool has(int e)const;
    friend std::ostream& operator<<(std::ostream& os, const SymSet& t);
};

class Trace
{
    static int call_level;

public:
    Trace(const std::string& text, const SymSet& s);
    ~Trace();
    static void prefix();
    static int trace_on;
    static int show_symbols;
};
#endif

