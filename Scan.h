#ifndef SCAN_H
#define SCAN_H

#include <unordered_map>
#include <climits>
#include <cctype>
#include <vector>
#include "Source.h"

enum SymType
{
    inputsy, outputsy,

    NKEYS, MAXKEY=outputsy,

    intconst, varname, becomes, andop, orop, notop, semicolon, others, MAXSYM=others+1
};

enum ScanErrors { ICONST2BIG = 1 };

class Scan
{
    static std::unordered_map<std::string, SymType> ktab;

    char c;
    TextPos atompos;
    unsigned long intconstant;
    std::string spell;

    void nextc()
    {
        c = src.nextChar();
    }


public:
    Source& src;

    explicit Scan(Source &s) : src(s), c(0), intconstant(0)
    {
        nextc();
    }
    SymType nextSymbol();
    unsigned long intConst()
    {
        return intconstant;
    }
    const std::string& getSpell()
    {
        return spell;
    }
    void scanError(int ecode, const std::string &mtxt = "", const std::string &atxt = "")
    {
        src.error(ecode, atompos, mtxt, atxt);
    }
};

#endif
