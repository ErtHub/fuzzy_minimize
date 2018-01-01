//
// Created by hubert on 08.12.17.
//

#ifndef FUZZY_MINIMIZE_MINIMIZER_H
#define FUZZY_MINIMIZE_MINIMIZER_H


#include <memory>
#include <chrono>
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
    virtual void report(std::ostream& os) const = 0;
};

class ExactMinimizer : public Minimizer
{
public:
    explicit ExactMinimizer(int w = 0) : Minimizer(w){};
    ~ExactMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    void report(std::ostream& os) const override { os << ""; };
};

class HeuristicMinimizer : public Minimizer
{
public:
    explicit HeuristicMinimizer(int w = 0) : Minimizer(w){};
    ~HeuristicMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    void report(std::ostream& os) const override { os << ""; };
};

class MukaidonoMinimizer : public Minimizer
{
public:
    explicit MukaidonoMinimizer(int w = 0) : Minimizer(w){};
    ~MukaidonoMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    void report(std::ostream& os) const override { os << ""; };
};

class MinimizerDecorator : public Minimizer
{
protected:
    std::shared_ptr<Minimizer> wrappee;
    explicit MinimizerDecorator(std::shared_ptr<Minimizer> minimizer) : wrappee(std::move(minimizer)){};
};

class Timer : public MinimizerDecorator
{
    std::list<double> timeRecords;
public:
    explicit Timer(std::shared_ptr<Minimizer> minimizer) : MinimizerDecorator(std::move(minimizer)){};
    ~Timer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    void report(std::ostream& os) const override;
};

#endif //FUZZY_MINIMIZE_MINIMIZER_H
