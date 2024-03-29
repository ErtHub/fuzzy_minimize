#ifndef FUZZY_MINIMIZE_FUNCTIONGENERATOR_H
#define FUZZY_MINIMIZE_FUNCTIONGENERATOR_H


#include "FuzzyFunction.h"
#include <random>
#include <fstream>

class FunctionGenerator
{
    unsigned long varCount, cubeCount, funCount;
    std::list<std::pair<std::string, FuzzyFunction>> result;
    VarTablePtr varTable;

    FuzzyFunction generateOne(unsigned long vCount, unsigned long cCount);

public:
    FunctionGenerator(unsigned long vCount, unsigned long cCount, unsigned long fCount) : varCount(vCount), cubeCount(cCount), funCount(fCount), varTable(new std::unordered_map<std::string, unsigned long>())
    {
        for(unsigned long i = 0; i < vCount; ++i)
            varTable->insert(std::make_pair(("z" + std::to_string(i)), i));
    };
    void generate();
    bool writeToFile(const std::string& fn) const;
};


#endif //FUZZY_MINIMIZE_FUNCTIONGENERATOR_H
