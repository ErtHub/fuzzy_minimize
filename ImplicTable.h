//
// Created by hubert on 23.07.17.
//

#ifndef FUZZY_MINIMIZE_IMPLICTABLE_H
#define FUZZY_MINIMIZE_IMPLICTABLE_H

#include "FuzzyFunction.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>

class ImplicTable
{
    std::list<std::vector<int>> content;

    std::list<unsigned long> localize1_2(const std::vector<int>& row, bool& found3) const;
    bool findRi(ImplicTable& sideList);
    std::list<std::tuple<unsigned long, unsigned long, std::vector<int>>> findR(std::vector<int>& r, ImplicTable& sideList, ImplicTable& ki);

public:

    ImplicTable() = default;
    explicit ImplicTable(const std::list<std::vector<int>> &content);
    explicit ImplicTable(const FuzzyFunction &func);
    void sweepCovered(ImplicTable& another);
    void sweepCovered();
    void sweepCovered(const std::vector<int>& i);
    void append(const std::vector<int>& item);
    bool checkCover(const std::vector<int> & covering, const std::vector<int>& covered) const;
    bool checkCover(const std::vector<int>& covered) const;
    ImplicTable generateK1() const;
    void chooseCoveringSubset();
    bool recursiveCover(std::vector<int>& implic, const std::list<std::vector<int>>& subset, std::list<unsigned long> positions0) const;
    void minimizeExact();
    void minimizeHeuristic();
    bool empty() const;
    void merge(ImplicTable& another);
    unsigned long long countLiterals(const std::vector<int>& implic) const;
    void print();
};


#endif //FUZZY_MINIMIZE_IMPLICTABLE_H
