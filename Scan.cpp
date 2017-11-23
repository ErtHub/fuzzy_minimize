// Modu Scan.cpp
// =================
// Defnicje funkcji skadowych klasy Scan, tabeli leksemw AT
// i tabeli sw kluczowych.
//
#include "Scan.h"

using namespace std;

vector<string> AT // Tabela Atomw (dla komunikatw o bdach
{
		"F", "input",
        "intconst", "varname",
        "=", "*", "+", "~", "others"
};

unordered_map<string, SymType> Scan::KT	// Kolejno wg funkcji hash(keyword)
{
// Keyword		 Atom			hash(keyword)
//-------------------------------------------
    { "input", inputsy },
    { "F", fsy }
};

SymType Scan::NextSymbol()
{

    while(isspace(c))
        Nextc();	// Pomi znaki biae
    if(c==EOF)
        return others;

    atompos=src.GetPos(); // Bdzie jaki atom leksykalny

    //---Identyfikator lub sowo kluczowe
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
            ScanError(ICONST2BIG, "Przekroczony zakres staej cakowitej");
        intconstant = (unsigned long)ul;
        return intconst;
    }
    else switch(c)
    {
	case '=':
        Nextc();
        return becomes;
    //----Operatory 1 znakowe
    case '+':
        Nextc();
        return orop;
    case '*':
        Nextc();
        return andop;
	case '~':
        Nextc();
        return notop;
    default:
        Nextc();
        return others;
  }
}
