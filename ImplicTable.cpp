//
// Created by hubert on 23.07.17.
//

#include "ImplicTable.h"

using namespace std;

ImplicTable::ImplicTable(const std::list<std::vector<int>> &content) : content(content)
{}

ImplicTable::ImplicTable(const FuzzyFunction &func) : content(func.tabulate())
{}

void ImplicTable::sweepCovered()
{
    for(auto i = content.begin(); i != content.end(); ++i)
        sweepCovered(i);
}

void ImplicTable::sweepCovered(list<vector<int>>::iterator i)
{
    for(auto k = content.begin(); k != content.end();)
    {
        if(k != i && checkCover(i, k))
            k = content.erase(k);
        else
            ++k;
    }
}

bool ImplicTable::checkCover(const list<vector<int>>::iterator covering, const list<vector<int>>::iterator covered) const
{
    for(unsigned pos = 0; pos < covering->size(); ++pos)
        if(covered->at(pos) & covering->at(pos) != covering->at(pos))
            return false;
    return true;
}