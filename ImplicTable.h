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
    std::list<ImplicRow> content;

//    std::list<unsigned long> localize1_2(const std::vector<int>& row, bool& found3) const;//implement inside
    bool findRi(ImplicTable& sideList);
    std::list<std::tuple<unsigned long, unsigned long, ImplicRow>> findR(ImplicRow& r, ImplicTable& sideList, ImplicTable& ki);

public:

    ImplicTable() = default;
    explicit ImplicTable(const std::list<ImplicRow> &content);
    explicit ImplicTable(const FuzzyFunction &func);
    void sweepCovered(ImplicTable& another);
    void sweepCovered();
    void sweepCovered(const ImplicRow& i);
    void append(const ImplicRow& item);
//    bool checkCover(const std::vector<int> & covering, const std::vector<int>& covered) const;//implement inside
    bool checkCover(const ImplicRow& covered) const;
    ImplicTable generateK1() const;
    void chooseCoveringSubset();
    bool recursiveCover(ImplicRow& implic, const std::list<ImplicRow>& subset, std::list<unsigned long>::iterator pos0, std::list<unsigned long>::iterator& pos0End) const;
    void minimizeExact();
    void minimizeHeuristic();
    bool empty() const;
    void merge(ImplicTable& another);
//    unsigned long long countLiterals(const std::vector<int>& implic) const;//implement inside
    void print();
};


#endif //FUZZY_MINIMIZE_IMPLICTABLE_H
