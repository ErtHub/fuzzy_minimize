#include "Scanner.h"

using namespace std;

unordered_map<string, TokenType> Scanner::keyTable
{
    { "input", inputsymb },
    { "output", outputsymb }
};

TokenTypeSet::TokenTypeSet(int elem) : content(vector<unsigned>(nwords, 0))
{
    if(elem >= 0 && elem < size)
        content[elem/ubits] = (unsigned)(1 << (elem % ubits));
}

TokenTypeSet::TokenTypeSet(int elem1, int elem2, ...) : content(vector<unsigned>(nwords, 0))
{
    va_list ap;
    int e;

    if(elem1 >= 0 && elem1 < size)
        content[elem1/ubits] = (unsigned)(1 << (elem1 % ubits));
    e = elem2;
    va_start(ap, elem2);
    while(e >= 0)
    {
        if(e < size)
            content[e/ubits] |= (1 << (e % ubits));
        e = va_arg(ap, int);
    }
    va_end(ap);
}

TokenTypeSet TokenTypeSet::operator+(const TokenTypeSet& set)const
{
    TokenTypeSet temp = *this;
    for(int i = 0; i < nwords; ++i)
        temp.content[i] |= set.content[i];
    return temp;
}

bool TokenTypeSet::contains(int elem) const
{
    if(elem < 0 || elem >= size)
        return false;
    return ((content[elem/ubits] & (1 << (elem % ubits))) != 0);
}

/*ostream& operator<< (ostream& os, const TokenTypeSet& set)
{
    unsigned word, bit, value, n = 0;
    os << '{';
    for(word = 0; word < set.nwords; ++word)
    {
        bit = 0;
        value = set.content[word];
        while(value)
        {
            if(value & 1)
            {
                if(n > 0) os << ',';
                os << tokenNames[(word * set.ubits + bit)];
                ++n;
            }
            value >>= 1;
            ++bit;
        }
    }
    os << '}';
    return os;
}*/

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
