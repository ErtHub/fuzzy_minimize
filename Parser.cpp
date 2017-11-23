#include "Parser.h"

using namespace std;

//==================
void Parser::Nexts()		// Pobranie nastpnego symbolu
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
void Parser::SyntaxError(int atom)	// Oczekiwany atom
{
	scn.ScanError(FirstSyntaxError + atom, "Spodziewany symbol: ", AT[atom]);
}
//=================================
void Parser::SyntaxError1(int atom)	// Nieoczekiwany atom
{
    scn.ScanError(FirstSyntaxError + atom, "Nieoczekiwany symbol: ", AT[atom]);
}

Parser::Parser(Scan& sc): scn(sc), varcount(0)
// Otwiera "GLOBAL" Scope
{
    Synchronize::p = this;    // Zwizanie z klas Synchronize

    ststart = SymSet(fsy, EOS);
    factstart = SymSet(varname, notop, EOS);
    mulops = SymSet(andop, EOS);
    factiter = factstart + mulops;
    addops = SymSet(orop, EOS);

    Nexts();          // Pobranie 1-go atomu
}
//==============
bool Parser::Program()
{
    clear();
    Trace x("Program", SymSet(-1));
    return VarDecl(ststart) && FunDecl((SymSet(others, EOS)));
}
//========================
bool Parser::VarDecl(const SymSet& fs)
{
    Trace x("VarDecl", fs);
    Synchronize s(SymSet(inputsy), fs);
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
    return true;
}
//========================
bool Parser::FunDecl(const SymSet& fs)
{
    Trace x("FunDecl", fs);
    Synchronize s(SymSet(fsy), fs);
    if(!can_parse)
        return false;
    accept(fsy);
    accept(becomes);
    return Sum(fs);
}
//========================
bool Parser::Sum(const SymSet& fs)
{
    Trace x("Sum", fs);
    Synchronize s(factstart, fs);
    if(!can_parse)
        return false;
    list<SymbInstance> implicProt;
    if(!factstart.has(symbol) || !Implicant(addops, implicProt))
        return false;
    else
        funProt.emplace_back(Implic(implicProt));
    while(symbol == orop)
    {
        implicProt.clear();
        accept(orop);
        if(Implicant(addops, implicProt))
            funProt.emplace_back(Implic(implicProt));
    }
    return true;
}
//========================
bool Parser::Implicant(const SymSet &fs, list<SymbInstance>& implicProt)
{
    Trace x("Implicant", fs);
    bool negative = false;
    if(symbol == notop)
    {
        accept(notop);
        negative = true;
    }
    if(symbol == varname)
    {
        implicProt.emplace_back(SymbInstance(scn.Spell(), negative));
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
            implicProt.emplace_back(SymbInstance(scn.Spell(), negative));
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
                    "Variable name collision",
                    "More variable names defined than declared",
                    "Less variable names defined than declared"
            };

    scn.ScanError(FirstSemanticError + ecode, SemErr[ecode]);
}

FuzzyFunction Parser::extract() const
{
    return FuzzyFunction(varTable, funProt);
}

void Parser::clear()
{
    varcount = 0;
    varTable.clear();
    funProt.clear();
}
//================================================
// Skadowa statyczna i funkcje  klasy Synchronize
//

Parser* Synchronize::p = nullptr;

Synchronize::Synchronize(const SymSet& sset, const SymSet& fset): f(fset)
{
    if(!sset.has(p->symbol))
    {
        p->SyntaxError1(p->symbol);	// Nieoczekiwany atom
        skipto(sset + f);
    }
    p->can_parse = sset.has(p->symbol);
}
//=========================
Synchronize::~Synchronize()
{
    if(!f.has(p->symbol))
    {
        p->SyntaxError1(p->symbol);   // Nieoczekiwany atom
        skipto(f);
    }
}
//========================================
void Synchronize::skipto(const SymSet &ss)
{
    while(!ss.has(p->symbol))
        p->Nexts();
}
