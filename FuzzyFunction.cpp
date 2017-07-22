//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(vector<double> args, OperationImpl* opImpl)
{
    return negative ? opImpl->negate(args[tableIndex]) : args[tableIndex];
}

SymbInstance::SymbInstance(unsigned int tableIndex, bool negative) : tableIndex(tableIndex), negative(negative)
{}

double Kos::calc(vector<double> args, OperationImpl* opImpl)
{
    double acc = 1;
    for(auto& i : content)
    {
        acc = opImpl->t_norm(acc, i.calc(args, opImpl));
    }
    return acc;
}

Kos::Kos(const list <SymbInstance> &content) : content(content)
{}

double FuzzyFunction::calc(vector<double> args, OperationImpl* opImpl)
{
    double acc = 0;
    for(auto& i : body)
    {
        acc = opImpl->s_norm(acc, i.calc(args, opImpl));
    }
    return acc;
}

FuzzyFunction::FuzzyFunction(const list <Kos> &body) : body(body)
{}
