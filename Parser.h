#ifndef PARSER_H
#define PARSER_H

#include "Scan.h"
#include "Util.h"
#include "FuzzyFunction.h"
#include <memory>

extern std::vector<std::string> AT;

class Parser;

class Synchronize
{
	friend class Parser;
    const SymSet &f;
    void skipto(const SymSet &ss);

public:
    static Parser* p;
    Synchronize(const SymSet& s,const SymSet& f);
    ~Synchronize();
};

class Parser
{
    friend class Synchronize;
    enum { FirstSyntaxError=10, FirstSemanticError=60 };

    Scan& scn;

    SymType symbol;
    bool can_parse;
    unsigned long varcount;
    std::unordered_map<std::string, unsigned long> varTable;
    std::list<Cube> funProt;


    SymSet ststart;
    SymSet factstart, mulops;
    SymSet addops;


    void Nexts();
    void accept(SymType atom);
    void SyntaxError(int atom);
    void SyntaxError1(int atom);


    bool VarDecl(const SymSet& fs);
    bool FunDecl(const SymSet& fs);
    bool Sum(const SymSet& fs);
    bool Implicant(const SymSet& fs, std::list<SymbInstance>& CubeProt);

public:

    bool Program();

    explicit Parser(Scan&);
    ~Parser() = default;
    void SemanticError(int ecode);
    FuzzyFunction extract() const;
    void clear();
};
#endif
