#ifndef PARSER_H
#define PARSER_H

#include "Scan.h"
#include "Util.h"
#include "FuzzyFunction.h"
#include <memory>

extern std::vector<std::string> AT;	// Atom Tab - dla komunikatw o bdach

class Parser;

class Synchronize   // Do synchronizacji parsera
{
	friend class Parser;
    const SymSet &f;
    void skipto(const SymSet &ss);

public:
    static Parser *p;
    Synchronize(const SymSet& s,const SymSet& f);
    ~Synchronize();
};

class Parser
{
    friend class Synchronize;
    enum { FirstSyntaxError=10, FirstSemanticError=60 };

    Scan& scn;

    SymType symbol; // Ostatnio pobrany atom z scanner'a
    bool can_parse;	// Jeli synchronizacja OK
    unsigned long varcount;
    std::unordered_map<std::string, unsigned long> varTable;
    std::list<Implic> funProt;

    // Podzbiory atomw
    SymSet ststart,   stseq,    stiter;
    SymSet factstart, factiter, mulops;
    SymSet addops,    signs,    relops, types;


    void Nexts();				         // Nastpny symbol
    void accept(SymType atom);
    void SyntaxError(int atom);	 // Oczekiwany atom
    void SyntaxError1(int atom); // Nieoczekiwany atom


    bool VarDecl(const SymSet& fs);
    bool FunDecl(const SymSet& fs);
    bool Sum(const SymSet& fs);
    bool Implicant(const SymSet& fs, std::list<SymbInstance>& implicProt);

public:

    bool Program();

    explicit Parser(Scan&);
    ~Parser() = default;
    void SemanticError(int ecode);
    FuzzyFunction extract() const;
    void clear();
};
#endif
