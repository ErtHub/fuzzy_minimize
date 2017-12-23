#include "Scanner.h"

using namespace std;

vector<string> atab
{
		"input", "output",
        "intconst", "varname",
        "=", "*", "+", "~", ";", "others"
};

unordered_map<string, SymType> Scanner::ktab
{
    { "input", inputsy },
    { "output", outputsy }
};//TODO to powinno chyba byc statyczne

SymType Scanner::nextSymbol()
{

    while(isspace(c))
        nextc();
    if(c==EOF)
        return others;

    atompos= src.getPos();

    if(isalpha(c))
    {
        spell.clear();

        do
        {
            spell.push_back(c);
            nextc();
        } while(isalnum(c));
        if(ktab.find(spell) != ktab.end())
            return ktab.at(spell);
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
            nextc();
        } while (isdigit(c));
        if (big)
            scanError(ICONST2BIG, "Integer overflow");
        intconstant = (unsigned long)ul;
        return intconst;
    }
    else switch(c)
    {
	case '=':
        nextc();
        return becomes;
    case '+':
        nextc();
        return orop;
    case '*':
        nextc();
        return andop;
	case '~':
        nextc();
        return notop;
    case ';':
        nextc();
        return semicolon;
    default:
        nextc();
        return others;
  }
}
