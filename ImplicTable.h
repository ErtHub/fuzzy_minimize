//
// Created by hubert on 23.07.17.
//

#ifndef FUZZY_MINIMIZE_IMPLICTABLE_H
#define FUZZY_MINIMIZE_IMPLICTABLE_H

#include "FuzzyFunction.h"
#include <list>
#include <vector>

class ImplicTable
{
    std::list<std::vector<int>> content;
public:

    explicit ImplicTable(const std::list<std::vector<int>> &content);
    explicit ImplicTable(const FuzzyFunction &func);
    void sweepCovered();
    void sweepCovered(std::list<std::vector<int>>::iterator i);
    bool checkCover(std::list<std::vector<int>>::iterator covering, std::list<std::vector<int>>::iterator covered) const;
};


#endif //FUZZY_MINIMIZE_IMPLICTABLE_H
