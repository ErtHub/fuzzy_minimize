//
// Created by hubert on 16.07.17.
//

#include "FuzzyFunction.h"

using namespace std;

double SymbInstance::calc(vector<double> args, OperationImpl opImpl = ZadehClassic())
{
    return negative ? opImpl.negate(args[tableIndex]) : args[tableIndex];
}

double Kos::calc(vector<double> args, OperationImpl opImpl)
{
    double acc = 1;
    for(auto& i : content)
    {
        acc = opImpl.t_norm(acc, i.calc(args, opImpl));
    }
    return acc;
}

double FuzzyFunction::calc(vector<double> args, OperationImpl opImpl)
{
    double acc = 0;
    for(auto& i : body)
    {
        acc = opImpl.s_norm(acc, i.calc(args, opImpl));
    }
    return acc;
}