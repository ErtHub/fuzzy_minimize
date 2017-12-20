//
// Created by hubert on 20.12.17.
//

#ifndef FUZZY_MINIMIZE_FUNCTIONGENERATOR_H
#define FUZZY_MINIMIZE_FUNCTIONGENERATOR_H


#include "FuzzyFunction.h"
#include <random>
#include <fstream>

class FunctionGenerator
{
    unsigned long varCount, cubeCount, funCount;
    std::list<std::pair<std::string, FuzzyFunction>> result;
    std::unordered_map<std::string, unsigned long> varTable;

    FuzzyFunction generateOne(unsigned long vCount, unsigned long cCount);

public:
    FunctionGenerator(unsigned long vCount, unsigned long cCount, unsigned long fCount = 1) : varCount(vCount), cubeCount(cCount), funCount(fCount) {};
    explicit FunctionGenerator(unsigned long vcCount = 1) : varCount(vcCount), cubeCount(vcCount), funCount(1) {};
    void generate();
    bool writeToFile(const std::string& fn) const;
};


#endif //FUZZY_MINIMIZE_FUNCTIONGENERATOR_H