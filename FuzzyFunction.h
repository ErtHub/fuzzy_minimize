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
    double calc(std::vector<double> args, OperationImpl* opImpl = &ZADEH_CLASSIC);

    SymbInstance(unsigned int tableIndex, bool negative);
};

class Kos
{
    std::list<SymbInstance> content;
public:
    double calc(std::vector<double> args, OperationImpl* opImpl = &ZADEH_CLASSIC);

    explicit Kos(const std::list<SymbInstance> &content);
};

class FuzzyFunction
{
    std::list<Kos> body;
public:
    double calc(std::vector<double> args, OperationImpl* opImpl = &ZADEH_CLASSIC);

    explicit FuzzyFunction(const std::list<Kos> &body);
};


#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
