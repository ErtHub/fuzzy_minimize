//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include <unordered_map>
#include "OperationImpl.h"
#include "CubeRow.h"
#include "CubeTable.h"
#include "Minimizer.h"


class CubeTable;
class Minimizer;
class ExactMinimizer;

class SymbInstance
{
    std::string varName;
    bool negative;
    double calc(const std::unordered_map<std::string, unsigned long>& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    void appendToTable(const std::unordered_map<std::string, unsigned long>& varTable, std::vector<uint8_t>& target) const;
public:
    SymbInstance(std::string varName, bool negative);

    bool operator==(const SymbInstance& another) const;
    friend class Cube;
    friend std::ostream& operator<<(std::ostream& os, const SymbInstance& s);
};

class Cube
{
    std::list<SymbInstance> content;
    double calc(const std::unordered_map<std::string, unsigned long>& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::vector<uint8_t> tabulate(const std::unordered_map<std::string, unsigned long>& varTable) const;
public:
    bool covers(const Cube& another) const;
    bool hasSymbol(const SymbInstance& symb) const;

    explicit Cube(const std::list<SymbInstance> &content);

    friend class FuzzyFunction;
    friend std::ostream& operator<<(std::ostream& os, const Cube& i);
};
//TODO operatory dodawania, ew. append?
class FuzzyFunction
{
    std::unordered_map<std::string, unsigned long> varTable;
    std::list<Cube> body;
    static ExactMinimizer defaultMinimizer;
public:
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::list<CubeRow> tabulate() const;
    FuzzyFunction minimize(Minimizer* minimizer = (Minimizer*)&defaultMinimizer) const;

    FuzzyFunction() = default;
    FuzzyFunction(std::unordered_map<std::string, unsigned long> varTable, std::list<Cube> body);
    FuzzyFunction(const std::unordered_map<std::string, unsigned long>& varTable, const CubeTable& tab);
    std::unordered_map<std::string, unsigned long> getVarTable() const;
    friend std::ostream& operator<<(std::ostream& os, const FuzzyFunction& f);
};

#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
