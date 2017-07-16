//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include "OperationImpl.h"

class SymbInstance
{
    unsigned tableIndex;
    bool negative;
public:
    double calc(std::vector<double> args, OperationImpl opImpl = ZadehClassic());
};

class Kos
{
    std::list<SymbInstance> content;
public:
    double calc(std::vector<double> args, OperationImpl opImpl = ZadehClassic());
};

class FuzzyFunction
{
    std::list<Kos> body;
public:
    double calc(std::vector<double> args, OperationImpl opImpl = ZadehClassic());
};


#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
