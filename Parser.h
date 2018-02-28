#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "FuzzyFunction.h"
#include <memory>

extern std::vector<std::string> tokenNames;

class Parser;

class Sync
{
    Parser* par;
    friend class Parser;
    const TokenTypeSet &follow;
    void fastForward(const TokenTypeSet &to);

public:
    Sync(Parser* p, const TokenTypeSet& fst, const TokenTypeSet& flw);
    ~Sync();
};

class Parser
{
    static std::vector<std::string> tokenNames;

    friend class Sync;

    enum ErrorTypes
    {
        ERROR_TYPE_SYNTAX_EXP = 16,
        ERROR_TYPE_SYNTAX_UNEXP = 64,
        ERROR_TYPE_SEMANTIC = 256
    };

    enum SemanticErrors
    {
        NAME_COLLISION = 0,
        NENOUGH_VARNAMES,
        UNDECLARED_VAR,
        NENOUGH_FUNNAMES,
        UNDECLARED_FUN
    };

    Scanner& scn;

    TokenType currentToken;
    bool good;
    unsigned long varcount;
    unsigned long funcount;
    VarTable varTable;
	std::unordered_map<std::string, bool> funTable;
    FunctionBody funProt;

    std::list<std::pair<std::string, FuzzyFunction>> funDefs;

    TokenTypeSet instart, outstart, funstart, factstart, addops;

    void nexttok();
    void accept(TokenType token);
    void syntaxErrorExpectedSymbol(int token, const std::string& what = "Syntax error");
    void syntaxErrorUnexpectedSymbol(int token, const std::string& what = "Syntax error");
    void semanticError(int errcode, const std::string& what = "Semantic error");

    bool parseVarDecl(const TokenTypeSet& follow);
	bool parseFunDecl(const TokenTypeSet& follow);
    bool parseFunDef(const TokenTypeSet& follow);
    bool parseSum(const TokenTypeSet& follow);
    bool parseProduct(const TokenTypeSet& follow, CubeCont& cubeProt);

public:

    bool parseProgram();
    explicit Parser(Scanner& s);
    ~Parser() = default;
    std::list<std::pair<std::string, FuzzyFunction>> extract();
    void clear();
};
#endif
