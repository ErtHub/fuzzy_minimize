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
    int write;

    bool findRi(CubeTable& sideList);
    std::list<std::tuple<unsigned long, unsigned long, CubeRow>> findR(CubeRow& r, CubeTable& sideList, CubeTable& ki);
    bool checkCover(const CubeRow& covered) const;
    CubeTable generateK1() const;
    void chooseCoveringSubset();
    bool recursiveCover(CubeRow& cube, const std::list<CubeRow>& subset, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    bool omissionAllowed(CubeRow& cube, unsigned long position) const;
    bool omissionAllowedRecursively(CubeRow& cube, unsigned long position, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;

public:
    CubeTable(int w = 0) : write(w){};
    explicit CubeTable(const std::list<CubeRow> &content, int w = 0);
    explicit CubeTable(const FuzzyFunction &func, int w = 0);
    void sweepCovered(CubeTable& another);
    void sweepCovered();
    void sweepCovered(const CubeRow& i);
    void minimizeExact();
    void minimizeHeuristic();
    void minimizeMukaidono();
    void append(const CubeRow& item);
    bool empty() const;
    void merge(CubeTable& another);
    std::list<Cube> redeem(const std::unordered_map<std::string, unsigned long>& tab) const;

    friend std::ostream& operator<<(std::ostream& os, const CubeTable& f);
};


#endif //FUZZY_MINIMIZE_CUBETABLE_H
