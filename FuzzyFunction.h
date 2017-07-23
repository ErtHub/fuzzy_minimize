//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include "OperationImpl.h"

//TODO: namespace FuzzyLogic maybe?

class SymbInstance
{
    unsigned tableIndex;
    bool negative;
public:
    double calc(std::vector<double> args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    void appendToTable(std::vector<int>& target) const;

    SymbInstance(unsigned int tableIndex, bool negative);
};

class Implic
{
    std::list<SymbInstance> content;
public:
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::vector<int> tabulate() const;

    explicit Implic(const std::list<SymbInstance> &content);
};

class FuzzyFunction
{
    std::list<Implic> body;
public:
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::list<std::vector<int>> tabulate() const;

    explicit FuzzyFunction(const std::list<Implic> &body);
};


#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
