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

list<unsigned long> ImplicTable::localize1_2(const vector<int>& row, bool& found3) const
{
    list<unsigned long> positions1_2;
    for(unsigned long pos = 0; pos < row.size(); ++pos)
    {
        if(row.at(pos))
        {
            if(row.at(pos) == 3)
                found3 = true;
            else
                positions1_2.push_back(pos);
        }
    }
    return positions1_2;
}

ImplicTable ImplicTable::generateK1() const
{
    ImplicTable result;

    for(auto i = content.begin(); i != content.end(); ++i)
    {
        bool found3 = false;
        list<unsigned long> positions1_2 = localize1_2(*i, found3);
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
    content = subset;//TODO std::move
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

void ImplicTable::minimizeHeuristic()
{
    sweepCovered();
    ImplicTable sideList;
    ImplicTable ki;
    while(findRi(sideList))
    {
        vector<int> r = *sideList.content.begin();
        list<tuple<unsigned long, unsigned long, vector<int>>> R = findR(r, sideList, ki);
        while(!R.empty())
        {
            unsigned long pos1_2 = get<1>(*R.begin());
            //TODO moze cos zrobic z tymi referencjami?
            vector<int>& rj = get<2>(*R.begin());
            R.pop_front();
            vector<int> rk(rj.size());
            for(unsigned long i = 0; i < rk.size(); ++i)
                rk[i] = r.at(i) | rj.at(i);
            rk[pos1_2] = 0;
            ki.content.push_front(rk);
            sweepCovered(ki.content.begin());
            sideList.sweepCovered(ki.content.begin());
            ki.sweepCovered(ki.content.begin());
            if(checkCover(ki.content.begin()) || sideList.checkCover(ki.content.begin()) || ki.checkCover(ki.content.begin()))//TODO
            {
                ki.content.pop_front();
                continue;
            }
            bool found3 = false;
            if(localize1_2(rk, found3).empty())
                break;
            if(countLiterals(r) < countLiterals(rk))//TODO
            {
                ki.content.pop_front();
                break;
            }
            r = rk;
            R = findR(r, sideList, ki);
        }
    }
    merge(sideList);
    merge(ki);
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

bool ImplicTable::findRi(ImplicTable& sideList)
{
    while(!content.empty())
    {
        vector<int> v;
        v = *content.begin();
        content.pop_front();
        sideList.content.push_front(v);
        bool found3 = false;
        if(!localize1_2(v, found3).empty() && found3)
            return true;
    }
    return false;
}

bool tupleCompare(tuple<unsigned long, unsigned long, vector<int>>& first, tuple<unsigned long, unsigned long, vector<int>>& second)
{
    return get<0>(first) > get<0>(second);
}

list<tuple<unsigned long, unsigned long, vector<int>>> ImplicTable::findR(vector<int>& r, ImplicTable& sideList, ImplicTable& ki)
{
    //TODO lambda?
    list<tuple<unsigned long, unsigned long, vector<int>>> result;
    for(auto& rn : content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                        continue;
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    if(seen1_2)
                        continue;
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
            if(seen1_2)
                result.push_back(make_tuple(zeroPairs, pos1_2, rn));
        }
    }
    for(auto& rn : sideList.content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                        continue;
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    if(seen1_2)
                        continue;
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
            if(seen1_2)
                result.push_back(make_tuple(zeroPairs, pos1_2, rn));
        }
    }
    for(auto& rn : ki.content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                        continue;
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    if(seen1_2)
                        continue;
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
            if(seen1_2)
                result.push_back(make_tuple(zeroPairs, pos1_2, rn));
        }
    }
    result.sort(tupleCompare);
    return result;
}
