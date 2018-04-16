//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(const VarTable& varTable, const vector<double>& args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[varTable.at(varName)]) : args[varTable.at(varName)];
}

void SymbInstance::appendToTable(const VarTable& varTable, CubeRow& target) const
{
    target.set(target.get(varTable.at(varName)) | 1 << (negative ? 0 : 1), varTable.at(varName));
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

CubeRow Cube::tabulate(const VarTable& varTable) const
{
    CubeRow res(varTable.size());
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

Cube Cube::operator+=(const SymbInstance &symb)
{
    content.emplace_back(symb);
    return *this;
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
    for(auto& i : body)
        res.push_back((i.tabulate(*varTable)));
    return res;
}

FuzzyFunction FuzzyFunction::operator+=(const Cube &cube)
{
    body.emplace_back(cube);
    return *this;
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