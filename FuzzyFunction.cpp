//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(vector<double> args, OperationImpl* opImpl) const
{
    return negative ? opImpl->negate(args[tableIndex]) : args[tableIndex];
}

void SymbInstance::appendToTable(std::vector<int> &target) const
{
    while(tableIndex <= target.size())
        target.push_back(0);
    target[tableIndex] |= 1 << (1 & negative);
}

SymbInstance::SymbInstance(unsigned int tableIndex, bool negative) : tableIndex(tableIndex), negative(negative)
{}

double Implic::calc(const vector<double> &args, OperationImpl* opImpl) const
{
    double acc = 1;
    for(auto& i : content)
        acc = opImpl->t_norm(acc, i.calc(args, opImpl));
    return acc;
}

vector<int> Implic::tabulate() const
{
    vector<int> res;
    for(auto& i : content)
        i.appendToTable(res);
    return res;
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

list<vector<int>> FuzzyFunction::tabulate() const
{
    list<vector<int>> res;
    vector<int> partialRes;
    unsigned long maxLen = 0;
    for(auto& i : body)
    {
        partialRes = i.tabulate();
        res.push_back(partialRes);
        maxLen = partialRes.size() > maxLen ? partialRes.size() : maxLen;
    }
    for(auto& i : res)
        while(i.size() < maxLen)
            i.push_back(0);
}

FuzzyFunction::FuzzyFunction(const list <Implic> &body) : body(body)
{}
