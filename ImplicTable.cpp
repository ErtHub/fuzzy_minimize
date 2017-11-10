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
    for(auto& i : content)
        another.sweepCovered(i);
    for(auto& i : another.content)
        sweepCovered(i);
}

void ImplicTable::sweepCovered()
{
    for(auto& i : content)
        sweepCovered(i);
}

void ImplicTable::sweepCovered(const vector<int>& i)
{
    for(auto k = content.begin(); k != content.end();)
    {
        if(addressof(*k) != addressof(i) && checkCover(i, *k))
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

bool ImplicTable::checkCover(const vector<int>& covered) const
{
    for(auto& i : content)
        if(addressof(i) != addressof(covered) && checkCover(i, covered))
            return true;
    return false;
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
//                cout << (i->at(pos) ^ j->at(pos)) << endl;
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
        auto divisionPos = positions0.begin();
        auto pos0End = positions0.end();
        halfImplic1[*divisionPos] = 1;
        halfImplic2[*divisionPos] = 2;
        if(!(recursiveCover(halfImplic1, subset, ++divisionPos, pos0End) && recursiveCover(halfImplic2, subset, divisionPos, pos0End)))
            subset.push_back(implic);
    }
    content = subset;//TODO std::move
}

bool ImplicTable::recursiveCover(vector<int>& implic, const list<vector<int>>& subset, list<unsigned long>::iterator pos0, list<unsigned long>::iterator& pos0End) const
{
    for(auto& i : content)
        if(checkCover(i, implic))
            return true;
    for(auto& i : subset)
        if(checkCover(i, implic))
            return true;
    if(pos0 == pos0End)
        return false;
    vector<int> implic2 = implic;
    implic[*pos0] = 1;
    implic2[*pos0] = 2;
    return recursiveCover(implic, subset, ++pos0, pos0End) && recursiveCover(implic2, subset, pos0, pos0End);
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
    ImplicTable history;
    while(findRi(sideList))
    {
        vector<int> r;
        r = sideList.content.back();
        list<tuple<unsigned long, unsigned long, vector<int>>> R = findR(r, sideList, ki);
        unsigned long long rLiteralsCount = countLiterals(r);
        while(!R.empty())
        {
            unsigned long pos1_2 = get<1>(*R.begin());
            vector<int> rj;
            rj = get<2>(R.front());
            R.pop_front();
            vector<int> rk(rj.size());
            for(unsigned long i = 0; i < rk.size(); ++i) {
                rk[i] = r.at(i) | rj.at(i);
                cout << "(" << r.at(i) << ", " << rj.at(i) << ")";}
            cout << endl;
            rk[pos1_2] = 0;
            ki.content.push_back(rk);
            sweepCovered(ki.content.back());
            sideList.sweepCovered(ki.content.back());
            ki.sweepCovered(ki.content.back());
            if(checkCover(ki.content.back()) || sideList.checkCover(ki.content.back()) || ki.checkCover(ki.content.back()))
            {
                ki.content.pop_back();
                continue;
            }
            bool found3 = false;
            if(localize1_2(rk, found3).empty())
                break;
            if(rLiteralsCount < countLiterals(rk) || history.checkCover(ki.content.back()))
            {
                ki.content.pop_back();
                break;
            }
//            cout << "Otwieram Matlaba!" << endl;
            history.content.push_front(rk);
            r = rk;
            R = findR(r, sideList, ki);
            rLiteralsCount = countLiterals(r);
            print();
            cout << endl;
            sideList.print();
            cout << endl;
            ki.print();
            cout << "----------------" << endl;
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
        v = content.front();
        content.pop_front();
        sideList.content.push_back(v);
        bool found3 = false;
        if(!localize1_2(v, found3).empty() && found3)
            return true;
    }
    return false;
}

/*bool tupleCompare(tuple<unsigned long, unsigned long, vector<int>>& first, tuple<unsigned long, unsigned long, vector<int>>& second)
{
    return get<0>(first) > get<0>(second);
}*/

list<tuple<unsigned long, unsigned long, vector<int>>> ImplicTable::findR(vector<int>& r, ImplicTable& sideList, ImplicTable& ki)
{
    list<tuple<unsigned long, unsigned long, vector<int>>> result;
    auto loop = [&](list<vector<int>>& l)
    {
        for(auto& rn : l)
        {
            unsigned long zeroPairs = 0;
            unsigned long pos1_2 = 0;
            bool seen1_2 = false;
            bool seenCross3 = false;
            for(unsigned long i = 0; i < rn.size(); ++i)
            {
                if(rn.at(i))
                {
                    if(rn.at(i) == 3)
                    {
                        if(r.at(i) != 3)
                        {
                            seenCross3 = true;
                            break;
                        }
                    }
                    else if((rn.at(i) ^ r.at(i)) == 3)
                    {
//                        cout << r.at(i) << " " << rn.at(i) << endl;
                        if(seen1_2)
                        {
                            seen1_2 = false;
                            break;
                        }
                        else
                        {
                            seen1_2 = true;
                            pos1_2 = i;
                        }
                    }
                }
                else if(!(r.at(i)))
                    ++zeroPairs;
            }
            if(seen1_2 && !seenCross3){
                result.push_back(make_tuple(zeroPairs, pos1_2, rn));
//                cout << pos1_2 << " " << zeroPairs << endl;
            }
        }
    };
    /*auto tupleCompare = [&](tuple<unsigned long, unsigned long, vector<int>>& first, tuple<unsigned long, unsigned long, vector<int>>& second)
    {
        return get<0>(first) > get<0>(second);
    };*/
    /*for(auto& rn : content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        bool seenCross3 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                    {
                        seenCross3 = true;
                        break;
                    }
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    cout << r.at(i) << " " << rn.at(i) << endl;
                    if(seen1_2)
                    {
                        seen1_2 = false;
                        break;
                    }
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
        }
        if(seen1_2 && !seenCross3){
            result.push_back(make_tuple(zeroPairs, pos1_2, rn));
//                cout << pos1_2 << " " << zeroPairs << endl;
        }
    }
    for(auto& rn : sideList.content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        bool seenCross3 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                    {
                        seenCross3 = true;
                        break;
                    }
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    if(seen1_2)
                        break;
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
        }
        if(seen1_2 && !seenCross3)
            result.push_back(make_tuple(zeroPairs, pos1_2, rn));
    }
    for(auto& rn : ki.content)
    {
        unsigned long zeroPairs = 0;
        unsigned long pos1_2 = 0;
        bool seen1_2 = false;
        bool seenCross3 = false;
        for(unsigned long i = 0; i < rn.size(); ++i)
        {
            if(rn.at(i))
            {
                if(rn.at(i) == 3)
                {
                    if(r.at(i) != 3)
                    {
                        seenCross3 = true;
                        break;
                    }
                }
                else if((rn.at(i) ^ r.at(i)) == 3)
                {
                    if(seen1_2)
                        break;
                    else
                    {
                        seen1_2 = true;
                        pos1_2 = i;
                    }
                }
            }
            else if(!(r.at(i)))
                ++zeroPairs;
        }
        if(seen1_2 && !seenCross3)
            result.push_back(make_tuple(zeroPairs, pos1_2, rn));
    }*/
    loop(content);
    loop(sideList.content);
    loop(ki.content);
    result.sort(
            [&](tuple<unsigned long, unsigned long, vector<int>>& first, tuple<unsigned long, unsigned long, vector<int>>& second)
                {
                    return get<0>(first) > get<0>(second);
                }
    );
    return result;
}

unsigned long long ImplicTable::countLiterals(const vector<int>& implic) const
{
    unsigned long long result = 0;
    for(auto& i : implic)
    {
        if(i > 0)
            ++result;
        if(i > 2)
            ++result;
    }
    return result;
}