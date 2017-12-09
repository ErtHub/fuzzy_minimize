//
// Created by hubert on 23.07.17.
//

#ifndef FUZZY_MINIMIZE_CubeTABLE_H
#define FUZZY_MINIMIZE_CubeTABLE_H

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

    bool findRi(CubeTable& sideList);
    std::list<std::tuple<unsigned long, unsigned long, CubeRow>> findR(CubeRow& r, CubeTable& sideList, CubeTable& ki);

public:

    CubeTable() = default;
    explicit CubeTable(const std::list<CubeRow> &content);
    explicit CubeTable(const FuzzyFunction &func);
    void sweepCovered(CubeTable& another);
    void sweepCovered();
    void sweepCovered(const CubeRow& i);
    void append(const CubeRow& item);
    bool checkCover(const CubeRow& covered) const;
    CubeTable generateK1() const;
    void chooseCoveringSubset();
    bool recursiveCover(CubeRow& Cube, const std::list<CubeRow>& subset, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    bool omissionAllowed(CubeRow& Cube, unsigned long position) const;
    bool omissionAllowedRecursively(CubeRow& Cube, unsigned long position, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    void minimizeExact();
    void minimizeHeuristic();
    void minimizeMukaidono();
    bool empty() const;
    void merge(CubeTable& another);
    void print();
    std::list<Cube> redeem(const std::unordered_map<std::string, unsigned long>& tab) const;
};


#endif //FUZZY_MINIMIZE_CubeTABLE_H
