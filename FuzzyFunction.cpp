//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(const unordered_map<string, unsigned long>& varTable, const vector<double>& args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[varTable.at(varName)]) : args[varTable.at(varName)];
}

void SymbInstance::appendToTable(const unordered_map<string, unsigned long>& varTable, std::vector<unsigned char> &target) const
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

double Implic::calc(const unordered_map<string, unsigned long>& varTable, const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 1;
    for(auto& i : content)
        acc = opImpl->t_norm(acc, i.calc(varTable, args, opImpl));
    return acc;
}

vector<unsigned char> Implic::tabulate(const std::unordered_map<std::string, unsigned long>& varTable) const
{
    vector<unsigned char> res(varTable.size(), 0);
    for(auto& i : content)
        i.appendToTable(varTable, res);
    return res;
}

bool Implic::covers(const Implic& another) const
{
    for(auto& i : content)
        if(!another.hasSymbol(i))
            return false;
    return true;
}

bool Implic::hasSymbol(const SymbInstance& symb) const
{
    for(auto& i : content)
        if(i == symb)
            return true;
    return false;
}

Implic::Implic(const list <SymbInstance> &content) : content(content)
{}

ostream& operator<<(ostream& os, const Implic& i)
{
    auto iter = content.begin();
    os << *iter;
    for(++iter; iter != content.end(); ++iter)
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

list<ImplicRow> FuzzyFunction::tabulate() const
{
    list<ImplicRow> res;
    vector<unsigned char> partialRes;
    for(auto& i : body)
    {
        partialRes = move(i.tabulate(varTable));
        res.emplace_back(ImplicRow(partialRes));
    }
    return res;
}

FuzzyFunction::FuzzyFunction(unordered_map<string, unsigned long> varTable, list<Implic> body) : varTable(move(varTable)), body(move(body))
{}

ostream& operator<<(ostream& os, const FuzzyFunction& f)
{
    auto iter = body.begin();
    os << *iter;
    for(++iter; iter != body.end(); ++iter)
        os << " + " << *iter;
    return os;
}