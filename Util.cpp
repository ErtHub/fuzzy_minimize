#include "Util.h"

using namespace std;

extern vector<string> tokenNames;
//===================
TokenTypeSet::TokenTypeSet(int e) : content(vector<unsigned>(nwords, 0))
{
    if(e >= 0 && e < size)
        content[e/ubits] = (unsigned)(1 << (e % ubits));
}
//================================
TokenTypeSet::TokenTypeSet(int e1, int e2,...) : content(vector<unsigned>(nwords, 0))
{
    va_list ap;
    int e;

    if(e1 >= 0 && e1 < size)
        content[e1/ubits] = (unsigned)(1 << (e1 % ubits));
    e = e2;
    va_start(ap, e2);
    while(e >= 0)
    {
        if(e < size)
            content[e/ubits] |= (1 << (e % ubits));
        e = va_arg(ap, int);
    }
    va_end(ap);
}
//============================================
TokenTypeSet TokenTypeSet::operator+(const TokenTypeSet& t)const
{
    TokenTypeSet temp = *this;
    for(int i = 0; i < nwords; ++i)
        temp.content[i] |= t.content[i];
    return temp;
}
//==========================
bool TokenTypeSet::contains(int e) const
{
    if(e < 0 || e >= size)
        return false;
    return ((content[e/ubits] & (1 << (e % ubits))) != 0);
}
//================================================
ostream& operator<< (ostream& os, const TokenTypeSet& t)
{
    unsigned word, bit, value, n = 0;
    os << '{';
    for(word = 0; word < t.nwords; ++word)
    {
        bit = 0;
        value = t.content[word];
        while(value)
        {
            if(value & 1)
            {
                if(n > 0) os << ',';
                os << tokenNames[(word * t.ubits + bit)];
                ++n;
            }
            value >>= 1;
            ++bit;
        }
    }
    os << '}';
    return os;
}

Trace::Trace(const string& text, const TokenTypeSet& s)
{
    if(traceOn)
    {
        prefix();
        cout << text;
        if(showSymbols)
            cout << ' ' << s;
        cout << endl << flush;
    }
    ++callLevel;
}
//=============
Trace::~Trace()
{
    --callLevel;
    if(traceOn)
    {
        prefix();
        cout << "return" << endl;
    }
}
//==================
void Trace::prefix()
{
    int i;
    for(i = 0; i < callLevel; ++i)
        cout << ".";
}
