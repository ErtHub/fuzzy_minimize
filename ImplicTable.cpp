//
// Created by hubert on 23.07.17.
//

#include "ImplicTable.h"

ImplicTable::ImplicTable(const std::list<std::vector<int>> &content) : content(content)
{}

ImplicTable::ImplicTable(const FuzzyFunction &func) : content(func.tabulate())
{}