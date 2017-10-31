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

class ImplicTable
{
    std::list<std::vector<int>> content;
public:

    ImplicTable() = default;
    explicit ImplicTable(const std::list<std::vector<int>> &content);
    explicit ImplicTable(const FuzzyFunction &func);
    void sweepCovered();
    void sweepCovered(std::list<std::vector<int>>::iterator i);
    void append(const std::vector<int>& item);
    bool checkCover(const std::vector<int> & covering, const std::vector<int>& covered) const;
    ImplicTable generateK1() const;
    void chooseCoveringSubset();
    bool recursiveCover(std::vector<int>& implic, const std::list<std::vector<int>>& subset, std::list<unsigned long> positions0) const;
    void minimizeExact();
    bool empty() const;
    void merge(ImplicTable& another);
    void print();
};


#endif //FUZZY_MINIMIZE_IMPLICTABLE_H
