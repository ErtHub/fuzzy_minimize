#include "Parser.h"

using namespace std;

//==================
void Parser::Nexts()
{
    symbol = scn.NextSymbol();
}
//===============================
void Parser::accept(SymType atom)
{
    if(symbol == atom)
        Nexts();
    else
        SyntaxError(atom);
}
//================================
void Parser::SyntaxError(int atom)
{
	scn.ScanError(FirstSyntaxError + atom, "Expected symbol: ", AT[atom]);
}
//=================================
void Parser::SyntaxError1(int atom)
{
    scn.ScanError(FirstSyntaxError + atom, "Unexpected symbol: ", AT[atom]);
}

Parser::Parser(Scan& sc): scn(sc), varcount(0)
{
    Synchronize::p = this;

    instart = SymSet(inputsy, others, EOS);
    outstart = SymSet(outputsy, others, EOS);
    funstart = SymSet(varname, others, EOS);
    factstart = SymSet(varname, notop, others, EOS);
    addops = SymSet(orop, others, EOS);

    Nexts();
}
//==============
bool Parser::Program()
{
    clear();
    Trace x("Program", SymSet(-1));
    return VarDecl(outstart) && FunDecl(funstart) && FunDef((SymSet(others, EOS)));
}
//========================
bool Parser::VarDecl(const SymSet& fs)
{
    Trace x("VarDecl", fs);
    Synchronize s(instart, fs);
    if(!can_parse)
        return false;
    accept(inputsy);
    if(symbol == intconst)
    {
        varcount = scn.IntConst();
        accept(intconst);
    }
    else
        return false;
    while(symbol == varname)
    {
        if(varTable.find(scn.Spell()) != varTable.end())
            SemanticError(0);
        else
            varTable.insert(make_pair(scn.Spell(), varTable.size()));
        accept(varname);
    }
    if(varTable.size() > varcount)
        SemanticError(1);
    else if(varTable.size() < varcount)
        SemanticError(2);
    accept(colon);
    return true;
}
//========================
bool Parser::FunDecl(const SymSet& fs)
{
    Trace x("FunDecl", fs);
    Synchronize s(outstart, fs);
    if(!can_parse)
        return false;
    accept(outputsy);
    if(symbol == intconst)
    {
        funcount = scn.IntConst();
        accept(intconst);
    }
    else
        return false;
    while(symbol == varname)
    {
        if(funTable.find(scn.Spell()) != funTable.end() || varTable.find(scn.Spell()) != varTable.end())
            SemanticError(0);
        else
            funTable.insert(make_pair(scn.Spell(), false));
        accept(varname);
    }
    if(funTable.size() > funcount)
        SemanticError(4);
    else if(funTable.size() < funcount)
        SemanticError(5);
    accept(colon);
    return true;
}
//========================
bool Parser::FunDef(const SymSet &fs)
{
    Trace x("FunDef", fs);
    Synchronize s(funstart, fs);
    if(!can_parse)
        return false;
    while(symbol == varname)
    {
        string name = scn.Spell();
        accept(varname);
        unordered_map<string, bool>::iterator iter;
        if((iter = funTable.find(name)) == funTable.end())
        {
            SemanticError(6);
            continue;
        }
        else if(iter->second || (varTable.find(name) != varTable.end()))
        {
            SemanticError(0);
            continue;
        }
        accept(becomes);
        if(!Sum(fs + funstart))
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
bool Parser::Sum(const SymSet& fs)
{
    Trace x("Sum", fs);
    Synchronize s(factstart, fs);
    if(!can_parse)
        return false;
    list<SymbInstance> cubeProt;
    if(!factstart.has(symbol) || !Product(addops, cubeProt))
        return false;
    else
        funProt.emplace_back(Cube(cubeProt));
    while(symbol == orop)
    {
        cubeProt.clear();
        accept(orop);
        if(Product(addops + funstart, cubeProt))
            funProt.emplace_back(Cube(cubeProt));
    }
    return true;
}
//========================
bool Parser::Product(const SymSet &fs, list<SymbInstance> &cubeProt)
{
    Trace x("Product", fs);
    bool negative = false;
    if(symbol == notop)
    {
        accept(notop);
        negative = true;
    }
    if(symbol == varname)
    {
        if(varTable.find(scn.Spell()) == varTable.end())
        {
            SemanticError(3);
            return false;
        }
        cubeProt.emplace_back(SymbInstance(scn.Spell(), negative));
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
            if(varTable.find(scn.Spell()) == varTable.end())
                SemanticError(3);
            else
                cubeProt.emplace_back(SymbInstance(scn.Spell(), negative));
        }
        accept(varname);
        negative = false;
    }
    return true;
}
//===================================
void Parser::SemanticError(int ecode)
{
    static vector<string> SemErr
            {
                    "Name collision",
                    "More variable names declared than should be",
                    "Fewer variable names declared than should be",
                    "Undeclared variable",
                    "More function names declared than should be",
                    "Fewer function names declared than should be",
                    "Undeclared function"
            };

    scn.ScanError(FirstSemanticError + ecode, SemErr[ecode]);
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
        p->SyntaxError1(p->symbol);
        skipto(sset + f);
    }
    p->can_parse = sset.has(p->symbol);
}
//=========================
Synchronize::~Synchronize()
{
    if(!f.has(p->symbol))
    {
        p->SyntaxError1(p->symbol);
        skipto(f);
    }
}
//========================================
void Synchronize::skipto(const SymSet &ss)
{
    while(!ss.has(p->symbol))
        p->Nexts();
}
