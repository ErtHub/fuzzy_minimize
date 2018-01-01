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

TokenType Scanner::nextToken()
{

    while(isspace(currentChar))
        nextc();
    if(currentChar == EOF)
        return others;

    tokenPos = rdr.getPos();

    if(isalpha(currentChar))
    {
        spell.clear();

        do
        {
            spell.push_back(currentChar);
            nextc();
        } while(isalnum(currentChar));
        if(keyTable.find(spell) != keyTable.end())
            return keyTable.at(spell);
        else
            return varname;
    }
    else if(isdigit(currentChar))
    {
        bool big = false;
        unsigned long long ul = 0;
        do
        {
            ul = ul * 10 + (currentChar - '0');
            big = big || ul > ULONG_MAX;
            nextc();
        } while (isdigit(currentChar));
        if (big)
            scanError(ICONST2BIG, "Integer overflow");
        intConstant = (unsigned long)ul;
        return intconst;
    }
    else switch(currentChar)
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
