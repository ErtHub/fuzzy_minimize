//
// Created by hubert on 23.07.17.
//

#include "ImplicTable.h"

using namespace std;

ImplicTable::ImplicTable(const std::list<std::vector<int>> &content) : content(content)
{}

ImplicTable::ImplicTable(const FuzzyFunction &func) : content(func.tabulate())
{}

void ImplicTable::sweepCovered(ImplicTable& another)
{
    sweepCovered();
    another.sweepCovered();
    for(auto i = content.begin(); i != content.end(); ++i)
        another.sweepCovered(i);
    for(auto i = another.content.begin(); i != another.content.end(); ++i)
        sweepCovered(i);
}

void ImplicTable::sweepCovered()
{
    for(auto i = content.begin(); i != content.end(); ++i)
        sweepCovered(i);
}

void ImplicTable::sweepCovered(list<vector<int>>::iterator i)
{
    for(auto k = content.begin(); k != content.end();)
    {
        if(k != i && checkCover(*i, *k))
            k = content.erase(k);
        else
            ++k;
    }
}

void ImplicTable::append(const vector<int>& item)
{
    content.push_back(item);
}

bool ImplicTable::checkCover(const vector<int>& covering, const vector<int>& covered) const
{
    for(unsigned pos = 0; pos < covering.size(); ++pos)
        if((covered.at(pos) & covering.at(pos)) != covering.at(pos))
            return false;
    return true;
}

ImplicTable ImplicTable::generateK1() const
{
    ImplicTable result;

    for(auto i = content.begin(); i != content.end(); ++i)
    {
        list<unsigned long> positions1_2;
        bool found3 = false;
        for(unsigned long pos = 0; pos < i->size(); ++pos)
        {
            if(i->at(pos))
            {
                if(i->at(pos) == 3)
                    found3 = true;
                else
                    positions1_2.push_back(pos);
            }
        }
        if(positions1_2.empty())
            continue;
        auto j = i;
        for(++j; j != content.end(); ++j)
        {
            for(auto &pos : positions1_2)
            {
                cout << (i->at(pos) ^ j->at(pos)) << endl;
                if((i->at(pos) ^ j->at(pos)) == 3 && (found3 || (find(j->begin(), j->end(), 3) != j->end())))
                {
                    vector<int> v(j->size());
                    for(unsigned long p = 0; p < v.size(); ++p) {
                        v[p] = i->at(p) | j->at(p);
                    cout << "(" << i->at(p) << ", " << j->at(p) << ")";}
                    cout << endl;
                    v[pos] = 0;
                    result.append(v);
                }
            }
        }
    }
    return result;
}

void ImplicTable::chooseCoveringSubset()
{
    list<vector<int>> subset;
    while(!content.empty())
    {
        vector<int> implic = *content.begin();
        content.pop_front();
        list<unsigned long> positions0;
        bool found3 = false;
        for(unsigned long i = 0; i < implic.size(); ++i)
        {
            if(implic.at(i) == 0)
                positions0.push_back(i);
            if(implic.at(i) == 3)
                found3 = true;
        }
        if(positions0.empty() || !found3)
        {
            subset.push_back(implic);
            continue;
        }
        vector<int> halfImplic1 = implic;
        vector<int> halfImplic2 = implic;
        unsigned long divisionPos = *positions0.begin();
        positions0.pop_front();
        halfImplic1[divisionPos] = 1;
        halfImplic2[divisionPos] = 2;
        if(!(recursiveCover(halfImplic1, subset, positions0) && recursiveCover(halfImplic2, subset, positions0)))
            subset.push_back(implic);
    }
    content = subset;
}

bool ImplicTable::recursiveCover(vector<int>& implic, const list<vector<int>>& subset, list<unsigned long> positions0) const
{
    for(auto& i : content)
        if(checkCover(i, implic))
            return true;
    for(auto& i : subset)
        if(checkCover(i, implic))
            return true;
    if(positions0.empty())
        return false;
    unsigned long pos = *positions0.begin();
    positions0.pop_front();
    vector<int> implic2 = implic;
    implic[pos] = 1;
    implic2[pos] = 2;
    return recursiveCover(implic, subset, positions0) && recursiveCover(implic2, subset, positions0);//TODO zmienic tam gdzie trzeba na unsigned long i poprawic to kopiowanie calej listy bo to jest dramat kurwa
}

void ImplicTable::minimizeExact()
{
    ImplicTable k1;
    bool wasEmpty = true;
    do
    {
        k1 = generateK1();
        sweepCovered(k1);
        wasEmpty = k1.empty();
        merge(k1);
        print();
        cout << "----------------" << endl;
        //sweepCovered();
    } while(!wasEmpty);
    chooseCoveringSubset();
}

bool ImplicTable::empty() const
{
    return content.empty();
}

void ImplicTable::merge(ImplicTable &another)
{
    content.merge(another.content);
}

void ImplicTable::print()
{
    for(auto& row : content)
    {
        for(auto& elem : row)
        {
            cout << elem << " ";
        }
        cout << endl;
    }
}