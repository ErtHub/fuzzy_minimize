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
};


#endif //FUZZY_MINIMIZE_IMPLICTABLE_H
