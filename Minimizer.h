//
// Created by hubert on 08.12.17.
//

#ifndef FUZZY_MINIMIZE_MINIMIZER_H
#define FUZZY_MINIMIZE_MINIMIZER_H


#include "FuzzyFunction.h"

class FuzzyFunction;

class Minimizer
{
public:
    virtual ~Minimizer() = default;
    virtual FuzzyFunction minimize(const FuzzyFunction& input) = 0;
};

class ExactMinimizer : public Minimizer
{
public:
    ExactMinimizer() = default;
    ~ExactMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

class HeuristicMinimizer : public Minimizer
{
public:
    HeuristicMinimizer() = default;
    ~HeuristicMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

class MukaidonoMinimizer : public Minimizer
{
public:
    MukaidonoMinimizer() = default;
    ~MukaidonoMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

#endif //FUZZY_MINIMIZE_MINIMIZER_H
