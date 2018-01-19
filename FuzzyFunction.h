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


class CubeTable;

//a class representing a literal in a two-level fuzzy function expression
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

//a class representing a product of literals
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
//a class representing a two-level expression, i.e. a sum of cubes
class FuzzyFunction
{
    std::unordered_map<std::string, unsigned long> varTable;
    std::list<Cube> body;

public:
    void clear();
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::list<CubeRow> tabulate() const;

    FuzzyFunction() = default;
    FuzzyFunction(std::unordered_map<std::string, unsigned long> varTable, std::list<Cube> body);
    FuzzyFunction(const std::unordered_map<std::string, unsigned long>& varTable, const CubeTable& tab);//TODO to chyba powinno byc zrobione inaczej...

    std::unordered_map<std::string, unsigned long> getVarTable() const;
    friend std::ostream& operator<<(std::ostream& os, const FuzzyFunction& f);
};

#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
