//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"
//TODO wypisywac 0 przy pustej funkcji
using namespace std;

double SymbInstance::calc(const unordered_map<string, unsigned long>& varTable, const vector<double>& args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[varTable.at(varName)]) : args[varTable.at(varName)];
}

void SymbInstance::appendToTable(const unordered_map<string, unsigned long>& varTable, std::vector<uint8_t> &target) const
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

double Cube::calc(const unordered_map<string, unsigned long>& varTable, const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 1;
    for(auto& i : content)
        acc = opImpl->t_norm(acc, i.calc(varTable, args, opImpl));
    return acc;
}

vector<uint8_t> Cube::tabulate(const std::unordered_map<std::string, unsigned long>& varTable) const
{
    vector<uint8_t> res(varTable.size(), 0);
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

Cube::Cube(const list<SymbInstance>& content) : content(content)
{}

ostream& operator<<(ostream& os, const Cube& i)
{
    auto iter = i.content.begin();
    os << *iter;
    for(++iter; iter != i.content.end(); ++iter)
        os << "*" << *iter;
    return os;
}

double FuzzyFunction::calc(const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 0;
    for(auto& i : body)
        acc = opImpl->s_norm(acc, i.calc(varTable, args, opImpl));
    return acc;
}

list<CubeRow> FuzzyFunction::tabulate() const
{
    list<CubeRow> res;
    vector<uint8_t> partialRes;
    for(auto& i : body)
    {
        partialRes = move(i.tabulate(varTable));
        res.emplace_back(CubeRow(partialRes));
    }
    return res;
}

FuzzyFunction FuzzyFunction::minimize(Minimizer* minimizer) const
{
    return minimizer->minimize(*this);
}

FuzzyFunction::FuzzyFunction(unordered_map<string, unsigned long> varTable, list<Cube> body) : varTable(move(varTable)), body(move(body))//move yo' body :D
{}

FuzzyFunction::FuzzyFunction(const unordered_map<string, unsigned long>& varTable, const CubeTable& tab) : body(move(tab.redeem(varTable))), varTable(varTable)
{}

unordered_map<string, unsigned long> FuzzyFunction::getVarTable() const
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