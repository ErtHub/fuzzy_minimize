#include "Parser.h"

using namespace std;

//==================
void Parser::nexts()
{
    symbol = scn.nextSymbol();
}
//===============================
void Parser::accept(SymType atom)
{
    if(symbol == atom)
        nexts();
    else
        syntaxErrorExpectedSymbol(atom);
}
//================================
void Parser::syntaxErrorExpectedSymbol(int atom)
{
    scn.scanError(FirstSyntaxError + atom, "Expected symbol: ", atab[atom]);
}
//=================================
void Parser::syntaxErrorUnexpectedSymbol(int atom)
{
    scn.scanError(FirstSyntaxError + atom, "Unexpected symbol: ", atab[atom]);
}

Parser::Parser(Scan& sc): scn(sc), varcount(0), funcount(0)
{
    Synchronize::p = this;

    instart = SymSet(inputsy, others, EOS);
    outstart = SymSet(outputsy, others, EOS);
    funstart = SymSet(varname, others, EOS);
    factstart = SymSet(varname, notop, others, EOS);
    addops = SymSet(orop, others, EOS);

    nexts();
}
//==============
bool Parser::parseProgram()
{
    clear();
    Trace x("parseProgram", SymSet(-1));
    return parseVarDecl(outstart) && parseFunDecl(funstart) && parseFunDef((SymSet(others, EOS)));
}
//========================
bool Parser::parseVarDecl(const SymSet &fs)//TODO bardziej generycznie?
{
    Trace x("parseVarDecl", fs);
    Synchronize s(instart, fs);
    if(!can_parse)
        return false;
    unsigned long declaredSoFar = 0;
    accept(inputsy);
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
bool Parser::parseFunDecl(const SymSet &fs)
{
    Trace x("parseFunDecl", fs);
    Synchronize s(outstart, fs);
    if(!can_parse)
        return false;
    unsigned long declaredSoFar = 0;
    accept(outputsy);
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
bool Parser::parseFunDef(const SymSet &fs)
{
    Trace x("parseFunDef", fs);
    Synchronize s(funstart, fs);
    if(!can_parse)
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
        if(!parseSum(fs + funstart))
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
bool Parser::parseSum(const SymSet &fs)
{
    Trace x("parseSum", fs);
    Synchronize s(factstart, fs);
    if(!can_parse)
        return false;
    list<SymbInstance> cubeProt;
    if(!factstart.has(symbol) || !parseProduct(addops, cubeProt))
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
bool Parser::parseProduct(const SymSet &fs, list<SymbInstance> &cubeProt)
{
    Trace x("parseProduct", fs);
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
void Parser::semanticError(int ecode)
{
    static vector<string> SemErr
            {
                    "Name collision",
                    "Fewer variable names declared than should be",
                    "Undeclared variable",
                    "Fewer function names declared than should be",
                    "Undeclared function"
            };

    scn.scanError(FirstSemanticError + ecode, SemErr[ecode]);
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

Parser* Synchronize::p = nullptr;

Synchronize::Synchronize(const SymSet& sset, const SymSet& fset): f(fset)
{
    if(!sset.has(p->symbol))
    {
        p->syntaxErrorUnexpectedSymbol(p->symbol);
        skipto(sset + f);
    }
    p->can_parse = sset.has(p->symbol);
}
//=========================
Synchronize::~Synchronize()
{
    if(!f.has(p->symbol))
    {
        p->syntaxErrorUnexpectedSymbol(p->symbol);
        skipto(f);
    }
}
//========================================
void Synchronize::skipto(const SymSet &ss)
{
    while(!ss.has(p->symbol))
        p->nexts();
}
