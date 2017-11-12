//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(vector<double> args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[tableIndex]) : args[tableIndex];
}

void SymbInstance::appendToTable(std::vector<unsigned char> &target) const
{
    while(tableIndex <= target.size())
        target.push_back(0);
    target[tableIndex] |= 1 << (1 & !negative);
}

SymbInstance::SymbInstance(unsigned int tableIndex, bool negative) : tableIndex(tableIndex), negative(negative)
{}

bool SymbInstance::operator==(const SymbInstance &another) const
{
    if(&another == this)
        return true;
    return (tableIndex == another.tableIndex && negative == another.negative);
}

double Implic::calc(const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 1;
    for(auto& i : content)
        acc = opImpl->t_norm(acc, i.calc(args, opImpl));
    return acc;
}

vector<unsigned char> Implic::tabulate() const
{
    vector<unsigned char> res;
    for(auto& i : content)
        i.appendToTable(res);
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

double FuzzyFunction::calc(const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 0;
    for(auto& i : body)
        acc = opImpl->s_norm(acc, i.calc(args, opImpl));
    return acc;
}

list<ImplicRow> FuzzyFunction::tabulate() const
{
    list<ImplicRow> res;
    list<vector<unsigned char>> rawRes;
    vector<unsigned char> partialRes;
    unsigned long maxLen = 0;
    for(auto& i : body)
    {
        partialRes = i.tabulate();
        rawRes.push_back(partialRes);
        maxLen = partialRes.size() > maxLen ? partialRes.size() : maxLen;
    }
    for(auto& i : rawRes)
    {
        while(i.size() < maxLen)
            i.push_back(0);
        res.emplace_back(ImplicRow(i));
    }
    return res;
}

FuzzyFunction::FuzzyFunction(const list <Implic> &body) : body(body)
{}
