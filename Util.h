#ifndef UTIL_H
#define UTIL_H

#include <cstdarg>		// Zmienna lista argumentow
#include "Scan.h"

const int EOS=-1;			    // Oznacznik koca listy elementw

class SymSet
{
    enum
    {
        size = MAXSYM,		// Elementy zbioru: 0..(size-1)
        ubits = (sizeof(unsigned) * CHAR_BIT),// Bity w unsigned
        nwords = ((size-1)/ubits+1)			    // Liczba sw
    };

    std::vector<unsigned> s;	// Bitowa reprezentacja zbioru

public:
    SymSet() : s(std::vector<unsigned>(nwords, 0)) {};       		    // Zbir pusty
    explicit SymSet(int e);		      // Zbir 1-elementowy

    SymSet(int, int,...);	  // Zbir wieloelementowy

    SymSet operator+(const SymSet& t) const; // s + t
    bool has(int e)const;	// Czy e naley do *this
    friend std::ostream& operator<< (std::ostream& os, const SymSet& t);
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

