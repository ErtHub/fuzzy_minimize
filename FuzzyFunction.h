//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_FUZZYFUNCTION_H
#define FUZZY_MINIMIZE_FUZZYFUNCTION_H

#include <list>
#include <vector>
#include <unordered_map>
#include "OperationImpl.h"
#include "ImplicRow.h"

//TODO: namespace FuzzyLogic maybe?

class SymbInstance
{
    std::string varName;
    bool negative;
    double calc(const std::unordered_map<std::string, unsigned long>& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    void appendToTable(const std::unordered_map<std::string, unsigned long>& varTable, std::vector<unsigned char>& target) const;
public:
    SymbInstance(std::string varName, bool negative);

    bool operator==(const SymbInstance& another) const;
    friend class Implic;
    friend std::ostream& operator<<(std::ostream& os, const SymbInstance& s);
};

class Implic
{
    std::list<SymbInstance> content;
    double calc(const std::unordered_map<std::string, unsigned long>& varTable, const std::vector<double>& args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::vector<unsigned char> tabulate(const std::unordered_map<std::string, unsigned long>& varTable) const;
public:
    bool covers(const Implic& another) const;
    bool hasSymbol(const SymbInstance& symb) const;

    explicit Implic(const std::list<SymbInstance> &content);

    friend class FuzzyFunction;
    friend std::ostream& operator<<(std::ostream& os, const Implic& i);
};

class FuzzyFunction
{
    std::unordered_map<std::string, unsigned long> varTable;
    std::list<Implic> body;
public:
    double calc(const std::vector<double> &args, OperationImpl* opImpl = &ZADEH_CLASSIC) const;
    std::list<ImplicRow> tabulate() const;

    explicit FuzzyFunction(std::unordered_map<std::string, unsigned long> varTable, std::list<Implic> body);
    friend std::ostream& operator<<(std::ostream& os, const FuzzyFunction& f);
};


#endif //FUZZY_MINIMIZE_FUZZYFUNCTION_H
