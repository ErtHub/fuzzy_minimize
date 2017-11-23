#include "Util.h"

using namespace std;

extern vector<string> AT;
//===================
SymSet::SymSet(int e) : s(vector<unsigned>(nwords, 0))				// Zbir 1-elementowy
{
    if(e >= 0 && e < size)
        s[e/ubits] = (unsigned)(1 << (e % ubits));
}
//================================
SymSet::SymSet(int e1, int e2,...) : s(vector<unsigned>(nwords, 0))	// Zbir wieloelementowy
{
    va_list ap;
    int e;

    if(e1 >= 0 && e1 < size)
        s[e1/ubits] = (unsigned)(1 << (e1 % ubits));
    e = e2;
    va_start(ap, e2);
    while(e >= 0)    // Lista argumentw koczona EOS==-1;
    {
        if(e < size)	// Elementy poza zakresem ==> ignoruj
            s[e/ubits] |= (1 << (e % ubits));
        e = va_arg(ap, int);
    }
    va_end(ap);
}
//============================================
SymSet SymSet::operator+(const SymSet& t)const	// Suma: s+t
{
    SymSet temp = *this;
    for(int i = 0; i < nwords; ++i)
        temp.s[i] |= t.s[i];
    return temp;
}
//==========================
bool SymSet::has(int e) const	// Naleenie: s.has(element)
{
    if(e < 0 || e >= size)
        return false;
    return ((s[e/ubits] & (1 << e % ubits)) != 0);
}
//================================================
ostream& operator<< (ostream& os, const SymSet& t)
{
    unsigned word, bit, value, n = 0;
    os << '{';				// Postac wydruku: {e1,...,en}
    for(word = 0; word < t.nwords; ++word)
    {
        bit = 0;     			// Numer bitu w sowie #i
        value = t.s[word];
        while(value)
        {
            if(value & 1)
            {
                if(n > 0) os << ',';	// Wyprowadz separator
                os << AT[(word * t.ubits + bit)];
                ++n;
            }
            value >>= 1;
            ++bit;
        }
    }
    return os << '}';
}

// =========================================
// Definicje funkcji skadowych klasy Trace.
//
Trace::Trace(const string& text, const SymSet& s)
{
    if(trace_on)
    {
        prefix();
        cout << text;
        if(show_symbols) cout << ' ' << s;
        cout << endl << flush;
    }
    ++call_level;
}
//=============
Trace::~Trace()
{
    --call_level;
    if(trace_on)
    {
        prefix();
        cout << "return" << endl;
    }
}
//==================
void Trace::prefix()
{
    int i;
    for(i = 0; i < call_level; ++i)
        cout << ".";
}
