//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(const VarTable& varTable, const vector<double>& args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[varTable.at(varName)]) : args[varTable.at(varName)];
}

void SymbInstance::appendToTable(const VarTable& varTable, CubeRowCont& target) const
{
    target[varTable.at(varName)] |= 1 << (1 & !negative);
}

SymbInstance::SymbInstance(string varName, bool negative) : varName(move(varName)), negative(negative)
{}

bool SymbInstance::operator==(const SymbInstance &another) const
{
    if(&another == this)
        return true;
    return (varName == another.varName && negative == another.negative);
}

ostream& operator<<(ostream& os, const SymbInstance& s)
{
    os << (s.negative ? "~" : "") << s.varName;
    return os;
}

double Cube::calc(const VarTable& varTable, const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 1;
    for(auto& i : content)
        acc = opImpl->t_norm(acc, i.calc(varTable, args, opImpl));
    return acc;
}

CubeRowCont Cube::tabulate(const VarTable& varTable) const
{
    CubeRowCont res(varTable.size(), 0);
    for(auto& i : content)
        i.appendToTable(varTable, res);
    return res;
}

bool Cube::covers(const Cube& another) const
{
    for(auto& i : content)
        if(!another.hasSymbol(i))
            return false;
    return true;
}

bool Cube::hasSymbol(const SymbInstance& symb) const
{
    for(auto& i : content)
        if(i == symb)
            return true;
    return false;
}

Cube::Cube(const CubeCont& content) : content(content)
{}

ostream& operator<<(ostream& os, const Cube& i)
{
    if(i.content.empty())
    {
        os << "1";
        return os;
    }
    auto iter = i.content.begin();
    os << *iter;
    for(++iter; iter != i.content.end(); ++iter)
        os << "*" << *iter;
    return os;
}

void FuzzyFunction::clear()
{
    varTable.reset();
    body.clear();
}

double FuzzyFunction::calc(const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 0;
    for(auto& i : body)
        acc = opImpl->s_norm(acc, i.calc(*varTable, args, opImpl));
    return acc;
}

CubeTableCont FuzzyFunction::tabulate() const
{
    CubeTableCont res;
    CubeRowCont partialRes;
    for(auto& i : body)
    {
        partialRes = move(i.tabulate(*varTable));
        res.emplace_back(CubeRow(partialRes));
    }
    return res;
}

FuzzyFunction::FuzzyFunction(VarTablePtr varTable, FunctionBody body) : varTable(move(varTable)), body(move(body))
{}

FuzzyFunction::FuzzyFunction(const VarTablePtr& varTable, const CubeTable& tab) : body(move(tab.redeem(*varTable))), varTable(varTable)
{}

VarTablePtr FuzzyFunction::getVarTable() const
{
    return varTable;
}

ostream& operator<<(ostream& os, const FuzzyFunction& f)
{
    if(f.body.empty())
    {
        os << "0";
        return os;
    }
    auto iter = f.body.begin();
    os << *iter;
    for(++iter; iter != f.body.end(); ++iter)
        os << " + " << *iter;
    return os;
}