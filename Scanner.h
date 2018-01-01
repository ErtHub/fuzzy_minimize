#ifndef SCAN_H
#define SCAN_H

#include <unordered_map>
#include <climits>
#include <cctype>
#include <vector>
#include "Reader.h"

enum TokenType
{
    inputsymb, outputsymb,

    NKEYS, MAXKEY=outputsymb,

    intconst, varname, becomes, andop, orop, notop, semicolon, others, MAXSYMB=others+1
};

enum ScanErrors { ICONST2BIG = 1 };

class Scanner
{
    static std::unordered_map<std::string, TokenType> keyTable;

    char c;
    TextPos tokenPos;
    unsigned long intConstant;
    std::string spell;

    void nextc()
    {
        c = src.nextChar();
    }


public:
    Reader& src;

    explicit Scanner(Reader& s) : src(s), c(0), intConstant(0)
    {
        nextc();
    }
    TokenType nextSymbol();
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
        src.alert(errcode, tokenPos, expl, what);
    }
};

#endif
