//
// Created by hubert on 23.07.17.
//

#ifndef FUZZY_MINIMIZE_CUBETABLE_H
#define FUZZY_MINIMIZE_CUBETABLE_H

#include "FuzzyFunction.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <functional>

class FuzzyFunction;
class Cube;

class CubeTable
{
    std::list<CubeRow> content;
    //this variable is checked to decide if the table should be printed to standard output at certain algorithm stages
    int write;//TODO zrobic jako public static, wyprowadzic konsensus, zlikwidowac & 2

    //find another r_i row which has at least one 3 value
    bool findRi(CubeTable& sideList);
    /*for the current r_i row, find the set of rows that have exactly one 1-2 value pair with r_i and have no 3 values
     * at other positions than r_i; sort them by descending 0 value pairs count*/
    std::list<std::tuple<unsigned long, unsigned long, CubeRow>> findR(CubeRow& r, CubeTable& sideList, CubeTable& ki);
    //check if the table has any single row that subsumes the row given as 'covered'
    bool checkCover(const CubeRow& covered, int checkEqual = NEQUAL | EQUAL) const;
    /*for a K tetrary table, generate a K1 table containing all the fuzzy consensus rows excluding ones that derive from
     * pairs of rows of which both are not complementary*/
    CubeTable generateK1() const;
    //apply the Kleen law to check if a cube is subsumed by any set of cubes of the function
    bool recursiveCover(CubeRow& original, CubeRow& cube, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End, const std::list<CubeRow>& secondList = std::list<CubeRow>()) const;
    bool omissionAllowed(CubeRow& cube, unsigned long position) const;
    bool omissionAllowedRecursively(CubeRow& cube, unsigned long position, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    void expandAndFilter(CubeRow& cube, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End, std::list<CubeRow>& target) const;

public:
    explicit CubeTable(int w = 0) : write(w){};
    explicit CubeTable(const std::list<CubeRow> &content, int w = 0);
    explicit CubeTable(const FuzzyFunction &func, int w = 0);
    //the sweepCovered functions are called to remove all cubes subsumed by any other single cube
    void sweepCovered(CubeTable& another, int sweepEqual = NEQUAL | EQUAL);
    void sweepCovered(int sweepEqual = NEQUAL | EQUAL);
    //sweep out the cubes subsumed by the cube given as 'i'
    bool sweepCovered(const CubeRow& i, int sweepEqual = NEQUAL | EQUAL);
    //A. Wielgus 2003 - algorithm 7.2
    void minimizeExact();
    //A. Wielgus 2003 - algorithm 7.3
    void minimizeHeuristic();
    //M. Mukaidono - "Fuzzy logical functions and their minimal and irredundant form"
    void minimizeMukaidono();
    void append(const CubeRow& item);
    bool empty() const;
    unsigned long size() const;
    void merge(CubeTable& another);
    void sort();
    void clear();
    CubeRow pop_front();
    //create a printable fuzzy expression object for a given variable names table
    std::list<Cube> redeem(const std::unordered_map<std::string, unsigned long>& tab) const;
    CubeTable crossProduct(const CubeTable& another) const;
    //of the given cubes set decide by definition, which are function's essential prime implicants, then separate and return them
    CubeTable separateEssentials();
    std::list<CubeRow> findUncoveredCompletes(const CubeTable& covering) const;

    friend std::ostream& operator<<(std::ostream& os, const CubeTable& f);
};


#endif //FUZZY_MINIMIZE_CUBETABLE_H
