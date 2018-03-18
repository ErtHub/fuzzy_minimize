//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include "OperationImpl.h"
#include "CubeRow.h"
#include "CubeTable.h"


class CubeTable;
using CubeTableCont = std::list<CubeRow>;
using VarTable = std::unordered_map<std::string, unsigned long>;
using VarTablePtr = std::shared_ptr<VarTable>;

//a class representing a literal in a two-level fuzzy function expression
class SymbInstance
{
    std::string varName;
    bool negative;

    double calc(const VarTable& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    void appendToTable(const VarTable& varTable, CubeRowCont& target) const;
public:
    SymbInstance(std::string varName, bool negative);

    bool operator==(const SymbInstance& another) const;
    friend class Cube;
    friend std::ostream& operator<<(std::ostream& os, const SymbInstance& s);
};

using CubeCont = std::list<SymbInstance>;

//a class representing a product of literals
class Cube
{
    CubeCont content;

    double calc(const VarTable& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    CubeRowCont tabulate(const VarTable& varTable) const;
public:
    bool covers(const Cube& another) const;
    bool hasSymbol(const SymbInstance& symb) const;

    explicit Cube(const CubeCont &content);

    friend class FuzzyFunction;
    friend std::ostream& operator<<(std::ostream& os, const Cube& i);
};

using FunctionBody = std::list<Cube>;
//a class representing a two-level expression, i.e. a sum of cubes
class FuzzyFunction
{
    VarTablePtr varTable;
    FunctionBody body;

public:
    void clear();
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    CubeTableCont tabulate() const;

    FuzzyFunction() = default;
    FuzzyFunction(VarTablePtr varTable, FunctionBody body);
    FuzzyFunction(const VarTablePtr& varTable, const CubeTable& tab);

    VarTablePtr getVarTable() const;
    friend std::ostream& operator<<(std::ostream& os, const FuzzyFunction& f);
};

#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
