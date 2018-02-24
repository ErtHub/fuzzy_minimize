//
// Created by hubert on 08.12.17.
//

#ifndef FUZZY_MINIMIZE_MINIMIZER_H
#define FUZZY_MINIMIZE_MINIMIZER_H


#include <memory>
#include <chrono>
#include "FuzzyFunction.h"

class FuzzyFunction;
class CubeTable;

class Minimizer
{
protected:
    int write;//TODO zliczanie generowanych kostek?
    FuzzyFunction result;
public:
    explicit Minimizer(int w = 0) : write(w){}
    virtual ~Minimizer() = default;
    virtual FuzzyFunction minimize(const FuzzyFunction& input) = 0;
    virtual std::ostream& writeResult(std::ostream& os) const;
    virtual std::ostream& report(std::ostream& os) const = 0;
    virtual void clear();
};

using MinimizerPtr = std::shared_ptr<Minimizer>;

class ExactMinimizer : public Minimizer
{
    CubeTable givenFunction;
    CubeTable essentials;
    std::vector<CubeRow> redundants;
    CubeTable solutions;
    std::list<FuzzyFunction> coverings;
public:
    explicit ExactMinimizer(int w = 0) : Minimizer(w){};
    ~ExactMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& writeResult(std::ostream& os) const override;
    std::ostream& report(std::ostream& os) const override { return os; };
    void clear() override;
};

class HeuristicMinimizer : public Minimizer
{
public:
    explicit HeuristicMinimizer(int w = 0) : Minimizer(w){};
    ~HeuristicMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& report(std::ostream& os) const override { return os; };
};

class MukaidonoMinimizer : public Minimizer
{
public:
    explicit MukaidonoMinimizer(int w = 0) : Minimizer(w){};
    ~MukaidonoMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& report(std::ostream& os) const override { return os; };
};

class MinimizerDecorator : public Minimizer
{
protected:
    MinimizerPtr wrappee;
    explicit MinimizerDecorator(MinimizerPtr minimizer) : wrappee(std::move(minimizer)){};
};

class Timer : public MinimizerDecorator
{
    std::list<double> timeRecords;
public:
    explicit Timer(MinimizerPtr minimizer) : MinimizerDecorator(std::move(minimizer)){};
    ~Timer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& writeResult(std::ostream& os) const override;
    std::ostream& report(std::ostream& os) const override;
};

#endif //FUZZY_MINIMIZE_MINIMIZER_H
