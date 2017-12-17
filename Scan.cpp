#include "Scan.h"

using namespace std;

vector<string> AT
{
		"input", "output",
        "intconst", "varname",
        "=", "*", "+", "~", ";", "others"
};

unordered_map<string, SymType> Scan::KT
{
    { "input", inputsy },
    { "output", outputsy }
};

SymType Scan::NextSymbol()
{

    while(isspace(c))
        Nextc();
    if(c==EOF)
        return others;

    atompos=src.GetPos();

    if(isalpha(c))
    {
        spell.clear();

        do
        {
            spell.push_back(c);
            Nextc();
        } while(isalnum(c));
        if(KT.find(spell) != KT.end())
            return KT.at(spell);
        else
            return varname;
    }
    else if(isdigit(c))
    {
        bool big = false;
        unsigned long long ul = 0;
        do
        {
            ul = ul * 10 + (c - '0');
            big = big || ul > ULONG_MAX;
            Nextc();
        } while (isdigit(c));
        if (big)
            ScanError(ICONST2BIG, "Integer overflow");
        intconstant = (unsigned long)ul;
        return intconst;
    }
    else switch(c)
    {
	case '=':
        Nextc();
        return becomes;
    case '+':
        Nextc();
        return orop;
    case '*':
        Nextc();
        return andop;
	case '~':
        Nextc();
        return notop;
    case ';':
        Nextc();
        return colon;
    default:
        Nextc();
        return others;
  }
}
