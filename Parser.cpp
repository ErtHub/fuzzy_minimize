#include "Parser.h"

using namespace std;


void Parser::nexttok()
{
    currentToken = scn.nextToken();
}

void Parser::accept(TokenType token)
{
    if(currentToken == token)
        nexttok();
    else
        syntaxErrorExpectedSymbol(token);
}

void Parser::syntaxErrorExpectedSymbol(int token, const string& what)
{
    scn.scanError(ERROR_TYPE_SYNTAX_EXP + token, "Expected token: " + tokenNames[token], what);
}

void Parser::syntaxErrorUnexpectedSymbol(int token, const string& what)
{
    scn.scanError(ERROR_TYPE_SYNTAX_UNEXP + token, "Unexpected token: " + tokenNames[token], what);
}

Parser::Parser(Scanner& s) : scn(s), varcount(0), funcount(0)
{
    instart = TokenTypeSet(inputsymb, others, EOS);
    outstart = TokenTypeSet(outputsymb, others, EOS);
    funstart = TokenTypeSet(varname, others, EOS);
    factstart = TokenTypeSet(varname, notop, others, EOS);
    addops = TokenTypeSet(orop, others, EOS);

    nexttok();
}

bool Parser::parseProgram()
{
    clear();
    return parseVarDecl(outstart) && parseFunDecl(funstart) && parseFunDef((TokenTypeSet(others, EOS)));
}

bool Parser::parseVarDecl(const TokenTypeSet &follow)//TODO bardziej generycznie?
{
    Sync s(this, instart, follow);
    if(!good)
        return false;
    unsigned long declaredSoFar = 0;
    accept(inputsymb);
    if(currentToken == intconst)
    {
        varcount = scn.intConst();
        accept(intconst);
    }
    else
        return false;
    while(declaredSoFar < varcount)
    {
        if(currentToken != varname)
        {
            semanticError(NENOUGH_VARNAMES);
            break;
        }
        if(varTable.find(scn.getSpell()) != varTable.end())
            semanticError(NAME_COLLISION);
        else
            varTable.insert(make_pair(scn.getSpell(), varTable.size()));
        accept(varname);
        ++declaredSoFar;
    }
    return true;
}

bool Parser::parseFunDecl(const TokenTypeSet &follow)
{
    Sync s(this, outstart, follow);
    if(!good)
        return false;
    unsigned long declaredSoFar = 0;
    accept(outputsymb);
    if(currentToken == intconst)
    {
        funcount = scn.intConst();
        accept(intconst);
    }
    else
        return false;
    while(declaredSoFar < funcount)
    {
        if(currentToken != varname)
        {
            semanticError(NENOUGH_FUNNAMES);
        }
        if(funTable.find(scn.getSpell()) != funTable.end() || varTable.find(scn.getSpell()) != varTable.end())
            semanticError(NAME_COLLISION);
        else
            funTable.insert(make_pair(scn.getSpell(), false));
        accept(varname);
        ++declaredSoFar;
    }
    return true;
}

bool Parser::parseFunDef(const TokenTypeSet &follow)
{
    Sync s(this, funstart, follow);
    if(!good)
        return false;
    while(currentToken == varname)
    {
        string name = scn.getSpell();
        accept(varname);
        unordered_map<string, bool>::iterator iter;
        if((iter = funTable.find(name)) == funTable.end())
        {
            semanticError(UNDECLARED_FUN);
            continue;
        }
        else if(iter->second || (varTable.find(name) != varTable.end()))
        {
            semanticError(NAME_COLLISION);
            continue;
        }
        accept(becomes);
        if(!parseSum(follow + funstart))
            continue;
        funDefs.emplace_back(make_pair(name, FuzzyFunction(varTable, funProt)));
        funProt.clear();
//        funTable.erase(iter);
//        funTable.insert(make_pair(name, true));
        iter->second = true;
    }
    return true;
}

bool Parser::parseSum(const TokenTypeSet &follow)
{
    Sync s(this, factstart, follow);
    if(!good)
        return false;
    list<SymbInstance> cubeProt;
    if(!factstart.contains(currentToken) || !parseProduct(addops, cubeProt))
        return false;
    else
        funProt.emplace_back(Cube(cubeProt));
    while(currentToken == orop)
    {
        cubeProt.clear();
        accept(orop);
        if(parseProduct(addops + funstart, cubeProt))
            funProt.emplace_back(Cube(cubeProt));
    }
    return true;
}

bool Parser::parseProduct(const TokenTypeSet &follow, list<SymbInstance> &cubeProt)
{
    bool negative = false;
    if(currentToken == notop)
    {
        accept(notop);
        negative = true;
    }
    if(currentToken == varname)
    {
        if(varTable.find(scn.getSpell()) == varTable.end())
        {
            semanticError(UNDECLARED_VAR);
            return false;
        }
        cubeProt.emplace_back(SymbInstance(scn.getSpell(), negative));
        accept(varname);
    }
    else
        return false;
    negative = false;
    while(currentToken == andop)
    {
        accept(andop);
        if(currentToken == notop)
        {
            accept(notop);
            negative = true;
        }
        if(currentToken == varname)
        {
            if(varTable.find(scn.getSpell()) == varTable.end())
                semanticError(UNDECLARED_VAR);
            else
                cubeProt.emplace_back(SymbInstance(scn.getSpell(), negative));
        }
        accept(varname);
        negative = false;
    }
    return true;
}

void Parser::semanticError(int errcode, const string& what)
{
    static vector<string> explTab
            {
                    "Name collision",
                    "Fewer variable names declared than should be",
                    "Undeclared variable",
                    "Fewer function names declared than should be",
                    "Undeclared function"
            };

    scn.scanError(ERROR_TYPE_SEMANTIC + errcode, explTab[errcode], what);
}

list<pair<string, FuzzyFunction>> Parser::extract()
{
    auto result = move(funDefs);
    clear();
    return result;
}

void Parser::clear()
{
    varcount = 0;
    funcount = 0;
    varTable.clear();
    funTable.clear();
    funProt.clear();
    funDefs.clear();
}

Sync::Sync(Parser* p, const TokenTypeSet& fst, const TokenTypeSet& flw) : par(p), follow(flw)
{
    if(!fst.contains(par->currentToken))
    {
        par->syntaxErrorUnexpectedSymbol(par->currentToken);
        fastForward(fst + follow);
    }
    par->good = fst.contains(par->currentToken);
}

Sync::~Sync()
{
    if(!follow.contains(par->currentToken))
    {
        par->syntaxErrorUnexpectedSymbol(par->currentToken);
        fastForward(follow);
    }
}

void Sync::fastForward(const TokenTypeSet &to)
{
    while(!to.contains(par->currentToken))
        par->nexttok();
}
