//
// Created by hubert on 08.12.17.
//

#ifndef FUZZY_MINIMIZE_MINIMIZER_H
#define FUZZY_MINIMIZE_MINIMIZER_H


#include "FuzzyFunction.h"

class FuzzyFunction;

class Minimizer
{
protected:
    int write;
public:
    explicit Minimizer(int w = 0) : write(w){}
    virtual ~Minimizer() = default;
    virtual FuzzyFunction minimize(const FuzzyFunction& input) = 0;
};

class ExactMinimizer : public Minimizer
{
public:
    explicit ExactMinimizer(int w = 0) : Minimizer(w){};
    ~ExactMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

class HeuristicMinimizer : public Minimizer
{
public:
    explicit HeuristicMinimizer(int w = 0) : Minimizer(w){};
    ~HeuristicMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

class MukaidonoMinimizer : public Minimizer
{
public:
    explicit MukaidonoMinimizer(int w = 0) : Minimizer(w){};
    ~MukaidonoMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
};

#endif //FUZZY_MINIMIZE_MINIMIZER_H
