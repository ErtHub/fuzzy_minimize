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

ImplicTable ImplicTable::generateK1() const
{
    ImplicTable result;

    for(auto i = content.begin(); i != content.end(); ++i)
    {
        list<int> positions1_2;
        bool found3 = false;
        for(int pos = 0; i->size(); ++pos)
        {
            if(i->at(pos))
            {
                if(i->at(pos) ^ 3)
                    positions1_2.push_back(pos);
                else
                    found3 = true;
            }
        }
        if(positions1_2.empty())
            continue;
        auto j = i;
        for(++j; j != content.end(); ++j)
        {
            int posOf1And2 = -1;
            for(auto &pos : positions1_2)
            {
                if(i->at(pos) ^ j->at(pos) == 3)
                {
                    posOf1And2 = pos;
                    break;
                }
            }
            if(posOf1And2 >= 0 && (found3 || (find(j->begin(), j->end(), 3) != j->end())))
            {
                vector<int> v(j->size());
                for(int pos = 0; pos < v.size(); ++pos)
                    v[pos] = i->at(pos) | j->at(pos);
                v[posOf1And2] = 0;
                result.push_back(v);//TODO
            }
        }
    }
    return result;
}