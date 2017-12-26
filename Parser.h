#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "Util.h"
#include "FuzzyFunction.h"
#include <memory>

extern std::vector<std::string> tokenNames;

class Parser;

class Sync
{
	friend class Parser;
    const TokenTypeSet &follow;
    void fastForward(const TokenTypeSet &to);

public:
    static Parser* p;
    Sync(const TokenTypeSet& fst, const TokenTypeSet& flw);
    ~Sync();
};

class Parser
{
    friend class Sync;
    enum { ErrorTypeSyntax=16, ErrorTypeSemantic=64 };

    enum SemanticErrors
    {
        NAME_COLLISION = 0,
        NENOUGH_VARNAMES,
        UNDECLARED_VAR,
        NENOUGH_FUNNAMES,
        UNDECLARED_FUN
    };

    Scanner& scn;

    SymType symbol;
    bool canParse;
    unsigned long varcount;
    unsigned long funcount;
    std::unordered_map<std::string, unsigned long> varTable;
	std::unordered_map<std::string, bool> funTable;
    std::list<Cube> funProt;

    std::list<std::pair<std::string, FuzzyFunction>> funDefs;


    TokenTypeSet instart, outstart;
    TokenTypeSet funstart;
    TokenTypeSet factstart;
    TokenTypeSet addops;


    void nexts();
    void accept(SymType token);
    void syntaxErrorExpectedSymbol(int token);
    void syntaxErrorUnexpectedSymbol(int token);

	//TODO uzupelnic komentarze gramatyka
    bool parseVarDecl(const TokenTypeSet &follow);
	bool parseFunDecl(const TokenTypeSet &follow);
    bool parseFunDef(const TokenTypeSet &follow);
    bool parseSum(const TokenTypeSet &follow);
    bool parseProduct(const TokenTypeSet &follow, std::list<SymbInstance> &cubeProt);

public:

    bool parseProgram();

    explicit Parser(Scanner&);
    ~Parser() = default;
    void semanticError(int errcode);
    std::list<std::pair<std::string, FuzzyFunction>> extract();
    void clear();
};
#endif
