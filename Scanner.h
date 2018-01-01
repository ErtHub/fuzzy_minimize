#ifndef SCAN_H
#define SCAN_H

#include <unordered_map>
#include <climits>
#include <cctype>
#include <cstdarg>
#include <vector>
#include "Reader.h"

enum TokenType
{
    inputsymb, outputsymb,

    NKEYS, MAXKEY=outputsymb,

    intconst, varname, becomes, andop, orop, notop, semicolon, others, MAXSYMB=others+1
};

const int EOS = -1;

class TokenTypeSet
{
    std::vector<unsigned> content;

public:
    static const int size = MAXSYMB;
    static const int ubits = sizeof(unsigned) * CHAR_BIT;
    static const int nwords = (size - 1) / ubits + 1;

    TokenTypeSet() : content(std::vector<unsigned>(nwords, 0)) {};
    explicit TokenTypeSet(int elem);

    TokenTypeSet(int elem1, int elem2, ...);

    TokenTypeSet operator+(const TokenTypeSet& set) const;
    bool contains(int elem)const;
    friend std::ostream& operator<<(std::ostream& os, const TokenTypeSet& set);
};

enum ScanErrors { ICONST2BIG = 1 };

class Scanner
{
    static std::unordered_map<std::string, TokenType> keyTable;

    char currentChar;
    TextPos tokenPos;
    unsigned long intConstant;
    std::string spell;

    void nextc()
    {
        currentChar = rdr.nextChar();
    }


public:
    Reader& rdr;

    explicit Scanner(Reader& s) : rdr(s), currentChar(0), intConstant(0)
    {
        nextc();
    }
    TokenType nextToken();
    unsigned long intConst()
    {
        return intConstant;
    }
    const std::string& getSpell()
    {
        return spell;
    }
    void scanError(int errcode, const std::string& expl = "", const std::string& what = "Scan error")
    {
        rdr.alert(errcode, tokenPos, expl, what);
    }
};

#endif
