#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
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

    void appendToTable(const VarTable& varTable, CubeRow& target) const;
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

    CubeRow tabulate(const VarTable& varTable) const;
public:
    Cube operator+=(const SymbInstance& symb);

    explicit Cube(const CubeCont &content = CubeCont());

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
    CubeTableCont tabulate() const;
    FuzzyFunction operator+=(const Cube& cube);

    FuzzyFunction() = default;
    explicit FuzzyFunction(VarTablePtr varTable, FunctionBody body = FunctionBody());
    FuzzyFunction(const VarTablePtr& varTable, const CubeTable& tab);

    VarTablePtr getVarTable() const;
    friend std::ostream& operator<<(std::ostream& os, const FuzzyFunction& f);
};

#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
