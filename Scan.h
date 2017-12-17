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

    intconst, varname, becomes, andop, orop, notop, colon, others, MAXSYM=others+1
};

enum ScanErrors { ICONST2BIG = 1 };

class Scan
{
    static std::unordered_map<std::string, SymType> KT;

    char c;
    TextPos atompos;
    unsigned long intconstant;
    std::string spell;

    void Nextc()
    {
        c = src.NextChar();
    }


public:
    Source& src;

    explicit Scan(Source &s) : src(s), c(0), intconstant(0)
    {
        Nextc();
    }
    SymType NextSymbol();
    unsigned long IntConst()
    {
        return intconstant;
    }
    const std::string& Spell()
    {
        return spell;
    }
    void ScanError(int ecode, const std::string& mtxt = "", const std::string& atxt = "")
    {
        src.Error(ecode, atompos, mtxt, atxt);
    }
};

#endif
