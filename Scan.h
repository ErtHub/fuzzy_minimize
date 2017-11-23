// Modu SCAN.H
// ===============
// Definicja klasy Scan i typu SymType.
//
#ifndef SCAN_H
#define SCAN_H

#include <unordered_map>
#include <climits>
#include <cctype>
#include <vector>
#include "Source.h"

enum SymType
{
    fsy, inputsy,// Keywords

    NKEYS, MAXKEY=inputsy,

    intconst, varname, becomes, andop, orop, notop, others, MAXSYM=others+1
};

enum ScanErrors { ICONST2BIG = 1 };

class Scan
{
    struct KeyRec   // Pomocniczy deskryptor sowa kluczowego
    {
        std::string kw;		// Sowo kluczowe
        SymType atom; // Odpowiadajcy mu atom
    };
    static std::unordered_map<std::string, SymType> KT;	// Tabela sw kluczowych

    char c;			    // Aktualny znak z src
    TextPos atompos;	  // Pozycja atomu w tekscie
    unsigned long intconstant;// Warto ostatniej staej liczbowej
    std::string spell;      // Ostatnio wczytany ident

    void Nextc()
    {
        c = src.NextChar();
    }  // Pobieranie znakw


public:
    Source& src;

    explicit Scan(Source &s) : src(s), c(0), intconstant(0)
    {
        Nextc();
    }
    SymType NextSymbol();		// Zwraca nastpny atom leksykalny
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
