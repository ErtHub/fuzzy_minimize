#include "Scanner.h"

using namespace std;

vector<string> tokenNames
{
		"input", "output",
        "intconst", "varname",
        "=", "*", "+", "~", ";", "others"
};

unordered_map<string, TokenType> Scanner::keyTable
{
    { "input", inputsymb },
    { "output", outputsymb }
};//TODO to powinno chyba byc statyczne

TokenType Scanner::nextSymbol()
{

    while(isspace(c))
        nextc();
    if(c == EOF)
        return others;

    tokenPos = src.getPos();

    if(isalpha(c))
    {
        spell.clear();

        do
        {
            spell.push_back(c);
            nextc();
        } while(isalnum(c));
        if(keyTable.find(spell) != keyTable.end())
            return keyTable.at(spell);
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
        intConstant = (unsigned long)ul;
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
