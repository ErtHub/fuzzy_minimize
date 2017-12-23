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
     * at other positions than r_i; sort them by descending 0 values count*/
    std::list<std::tuple<unsigned long, unsigned long, CubeRow>> findR(CubeRow& r, CubeTable& sideList, CubeTable& ki);
    //check if the table has any single row that subsumes the row given as 'covered'
    bool checkCover(const CubeRow& covered) const;
    /*for a K tetrary table, generate a K1 table containing all the fuzzy consensus rows excluding ones that derive from
     * pairs of rows of which both are not complementary*/
    CubeTable generateK1() const;
    //of the given cubes set exclude the redundant complementary cubes leaving the function in one of its minimal forms
    void chooseCoveringSubset();
    //apply the Kleen law to check if a cube is subsumed by any set of cubes of the function
    bool recursiveCover(CubeRow& cube, const std::list<CubeRow>& subset, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    bool omissionAllowed(CubeRow& cube, unsigned long position) const;
    bool omissionAllowedRecursively(CubeRow& cube, unsigned long position, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;

public:
    explicit CubeTable(int w = 0) : write(w){};
    explicit CubeTable(const std::list<CubeRow> &content, int w = 0);
    explicit CubeTable(const FuzzyFunction &func, int w = 0);
    //the sweepCovered functions are called to remove all cubes subsumed by any other single cube
    void sweepCovered(CubeTable& another);
    void sweepCovered();
    //sweep out the cubes subsumed by the cube given as 'i'
    void sweepCovered(const CubeRow& i);
    //A. Wielgus 2003 - algorithm 7.2
    void minimizeExact();
    //A. Wielgus 2003 - algorithm 7.3
    void minimizeHeuristic();
    //M. Mukaidono - "Fuzzy logical functions and their minimal and irredundant form"
    void minimizeMukaidono();
    void append(const CubeRow& item);
    bool empty() const;
    void merge(CubeTable& another);
    //create a printable fuzzy function object for a given variable names table
    std::list<Cube> redeem(const std::unordered_map<std::string, unsigned long>& tab) const;

    friend std::ostream& operator<<(std::ostream& os, const CubeTable& f);
};


#endif //FUZZY_MINIMIZE_CUBETABLE_H
