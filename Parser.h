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


    void nexts();
    void accept(SymType atom);
    void syntaxErrorExpectedSymbol(int atom);
    void syntaxErrorUnexpectedSymbol(int atom);


    bool parseVarDecl(const SymSet &fs);
	bool parseFunDecl(const SymSet &fs);
    bool parseFunDef(const SymSet &fs);
    bool parseSum(const SymSet &fs);
    bool parseProduct(const SymSet &fs, std::list<SymbInstance> &cubeProt);

public:

    bool parseProgram();

    explicit Parser(Scan&);
    ~Parser() = default;
    void semanticError(int ecode);
    std::list<std::pair<std::string, FuzzyFunction>> extract();
    void clear();
};
#endif
