#ifndef FUZZY_MINIMIZE_MINIMIZER_H
#define FUZZY_MINIMIZE_MINIMIZER_H


#include <memory>
#include <chrono>
#include "FuzzyFunction.h"

class FuzzyFunction;
class CubeTable;

enum MinimizationFashion
{
    EXACT = 0, HEURISTIC, HEURISTIC_MUKAIDONO
};

class Minimizer
{
protected:
    int write;
    bool expand;
    FuzzyFunction result;
public:
    explicit Minimizer(int w = 0, bool x = false) : write(w), expand(x){}
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

    void solveColumnCovering();
    void solutionsToFunctions(const VarTablePtr& varTable);
public:
    explicit ExactMinimizer(int w = 0, bool x = false) : Minimizer(w, x){};
    ~ExactMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& writeResult(std::ostream& os) const override;
    std::ostream& report(std::ostream& os) const override { return os; };
    void clear() override;
};

class HeuristicMinimizer : public Minimizer
{
public:
    explicit HeuristicMinimizer(int w = 0, bool x = false) : Minimizer(w, x){};
    ~HeuristicMinimizer() override = default;
    FuzzyFunction minimize(const FuzzyFunction& input) override;
    std::ostream& report(std::ostream& os) const override { return os; };
};

class MukaidonoMinimizer : public Minimizer
{
public:
    explicit MukaidonoMinimizer(int w = 0, bool x = false) : Minimizer(w, x){};
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
