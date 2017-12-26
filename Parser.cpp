#include "Parser.h"

using namespace std;

//==================
void Parser::nexts()
{
    symbol = scn.nextSymbol();
}
//===============================
void Parser::accept(TokenType token)
{
    if(symbol == token)
        nexts();
    else
        syntaxErrorExpectedSymbol(token);
}
//================================
void Parser::syntaxErrorExpectedSymbol(int token)
{
    scn.scanError(ERROR_TYPE_SYNTAX + token, "Expected token: " + tokenNames[token]);
}//TODO enum, ktory rozroznia expected i unexpected
//=================================
void Parser::syntaxErrorUnexpectedSymbol(int token)
{
    scn.scanError(ERROR_TYPE_SYNTAX + token, "Unexpected token: " + tokenNames[token]);
}

Parser::Parser(Scanner& sc): scn(sc), varcount(0), funcount(0)
{
    Sync::p = this;

    instart = TokenTypeSet(inputsymb, others, EOS);
    outstart = TokenTypeSet(outputsymb, others, EOS);
    funstart = TokenTypeSet(varname, others, EOS);
    factstart = TokenTypeSet(varname, notop, others, EOS);
    addops = TokenTypeSet(orop, others, EOS);

    nexts();
}
//==============
bool Parser::parseProgram()
{
    clear();
    Trace x("parseProgram", TokenTypeSet(-1));
    return parseVarDecl(outstart) && parseFunDecl(funstart) && parseFunDef((TokenTypeSet(others, EOS)));
}
//========================
bool Parser::parseVarDecl(const TokenTypeSet &follow)//TODO bardziej generycznie?
{
    Trace x("parseVarDecl", follow);
    Sync s(instart, follow);
    if(!canParse)
        return false;
    unsigned long declaredSoFar = 0;
    accept(inputsymb);
    if(symbol == intconst)
    {
        varcount = scn.intConst();
        accept(intconst);
    }
    else
        return false;
    while(declaredSoFar < varcount)
    {
        if(symbol != varname)
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
//========================
bool Parser::parseFunDecl(const TokenTypeSet &follow)
{
    Trace x("parseFunDecl", follow);
    Sync s(outstart, follow);
    if(!canParse)
        return false;
    unsigned long declaredSoFar = 0;
    accept(outputsymb);
    if(symbol == intconst)
    {
        funcount = scn.intConst();
        accept(intconst);
    }
    else
        return false;
    while(declaredSoFar < funcount)
    {
        if(symbol != varname)
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
//========================
bool Parser::parseFunDef(const TokenTypeSet &follow)
{
    Trace x("parseFunDef", follow);
    Sync s(funstart, follow);
    if(!canParse)
        return false;
    while(symbol == varname)
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
//========================
bool Parser::parseSum(const TokenTypeSet &follow)
{
    Trace x("parseSum", follow);
    Sync s(factstart, follow);
    if(!canParse)
        return false;
    list<SymbInstance> cubeProt;
    if(!factstart.contains(symbol) || !parseProduct(addops, cubeProt))
        return false;
    else
        funProt.emplace_back(Cube(cubeProt));
    while(symbol == orop)
    {
        cubeProt.clear();
        accept(orop);
        if(parseProduct(addops + funstart, cubeProt))
            funProt.emplace_back(Cube(cubeProt));
    }
    return true;
}
//========================
bool Parser::parseProduct(const TokenTypeSet &follow, list<SymbInstance> &cubeProt)
{
    Trace x("parseProduct", follow);
    bool negative = false;
    if(symbol == notop)
    {
        accept(notop);
        negative = true;
    }
    if(symbol == varname)
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
    while(symbol == andop)
    {
        accept(andop);
        if(symbol == notop)
        {
            accept(notop);
            negative = true;
        }
        if(symbol == varname)
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
//===================================
void Parser::semanticError(int errcode)
{
    static vector<string> SemErr
            {
                    "Name collision",
                    "Fewer variable names declared than should be",
                    "Undeclared variable",
                    "Fewer function names declared than should be",
                    "Undeclared function"
            };

    scn.scanError(ERROR_TYPE_SEMANTIC + errcode, SemErr[errcode]);
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

Parser* Sync::p = nullptr;

Sync::Sync(const TokenTypeSet& fst, const TokenTypeSet& flw): follow(flw)
{
    if(!fst.contains(p->symbol))
    {
        p->syntaxErrorUnexpectedSymbol(p->symbol);
        fastForward(fst + follow);
    }
    p->canParse = fst.contains(p->symbol);
}
//=========================
Sync::~Sync()
{
    if(!follow.contains(p->symbol))
    {
        p->syntaxErrorUnexpectedSymbol(p->symbol);
        fastForward(follow);
    }
}
//========================================
void Sync::fastForward(const TokenTypeSet &to)
{
    while(!to.contains(p->symbol))
        p->nexts();
}
