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
    unsigned long funcount;
    std::unordered_map<std::string, unsigned long> varTable;
	std::unordered_map<std::string, bool> funTable;
    std::list<Cube> funProt;

    std::list<std::pair<std::string, FuzzyFunction>> funDefs;


    SymSet instart, outstart;
    SymSet funstart;
    SymSet factstart;
    SymSet addops;


    void Nexts();
    void accept(SymType atom);
    void SyntaxErrorExpectedSymbol(int atom);
    void SyntaxErrorUnexpectedSymbol(int atom);


    bool VarDecl(const SymSet& fs);
	bool FunDecl(const SymSet& fs);
    bool FunDef(const SymSet &fs);
    bool Sum(const SymSet& fs);
    bool Product(const SymSet &fs, std::list<SymbInstance> &cubeProt);

public:

    bool Program();

    explicit Parser(Scan&);
    ~Parser() = default;
    void SemanticError(int ecode);
    std::list<std::pair<std::string, FuzzyFunction>> extract();
    void clear();
};
#endif
