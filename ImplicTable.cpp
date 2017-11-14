//
// Created by hubert on 23.07.17.
//

#include "ImplicTable.h"

using namespace std;

ImplicTable::ImplicTable(const std::list<ImplicRow> &content) : content(content)
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

void ImplicTable::sweepCovered(const ImplicRow& i)
{
    for(auto k = content.begin(); k != content.end();)
    {
        if(addressof(*k) != addressof(i) && i.covers(*k))
            k = content.erase(k);
        else
            ++k;
    }
}

void ImplicTable::append(const ImplicRow& item)
{
    content.push_back(item);
}

bool ImplicTable::checkCover(const ImplicRow& covered) const
{
    for(auto& i : content)
        if(addressof(i) != addressof(covered) && i.covers(covered))
            return true;
    return false;
}

ImplicTable ImplicTable::generateK1() const
{
    ImplicTable result;

    for(auto i = content.begin(); i != content.end(); ++i)
    {
        list<unsigned long> positions1_2 = move(i->localize1_2());
        if(positions1_2.empty())
            continue;
        auto j = i;
        for(++j; j != content.end(); ++j)
        {
            if(i->get_meta_phase_number(3) || j->get_meta_phase_number(3))
            {
                for(auto &pos : positions1_2)
                {
                    if((i->get(pos) ^ j->get(pos)) == 3)
                    {
                        ImplicRow v(j->size());
                        for(unsigned long p = 0; p < v.size(); ++p) {
                            v.set(i->get(p) | j->get(p), p);
                            cout << "(" << (int)i->get(p) << ", " << (int)j->get(p) << ")";}
                        cout << endl;
                        v.set(0, pos);
                        result.append(v);
                    }
                }
            }
        }
    }
    return result;
}

void ImplicTable::chooseCoveringSubset()
{
    list<ImplicRow> subset;
    while(!content.empty())
    {
        ImplicRow implic = move(content.front());
        content.pop_front();
        list<unsigned long> positions0;
        if(!implic.get_meta_phase_number(0) || !implic.get_meta_phase_number(3))
        {
            subset.push_back(move(implic));
            continue;
        }
        for(unsigned long i = 0; i < implic.size(); ++i)
            if(implic.get(i) == 0)
                positions0.push_back(i);
        auto divisionPos = positions0.begin();
        auto pos0End = positions0.end();
        ImplicRow halfImplic1 = implic;
        ImplicRow halfImplic2 = move(halfImplic1.expand(*divisionPos));
        if(!(recursiveCover(halfImplic1, subset, ++divisionPos, pos0End) && recursiveCover(halfImplic2, subset, divisionPos, pos0End)))
            subset.push_back(move(implic));
    }
    content = move(subset);
}

bool ImplicTable::recursiveCover(ImplicRow& implic, const list<ImplicRow>& subset, list<unsigned long>::iterator pos0, list<unsigned long>::iterator& pos0End) const
{
    for(auto& i : content)
        if(i.covers(implic))
            return true;
    for(auto& i : subset)
        if(i.covers(implic))
            return true;
    if(pos0 == pos0End)
        return false;
    ImplicRow implic2 = move(implic.expand(*pos0));

    return recursiveCover(implic, subset, ++pos0, pos0End) && recursiveCover(implic2, subset, pos0, pos0End);
}

void ImplicTable::minimizeExact()
{
    content.sort();
    ImplicTable k1;
    bool wasEmpty = true;
    do
    {
        k1 = move(generateK1());
        k1.content.sort();
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
    content.sort();
    sweepCovered();
    ImplicTable sideList;
    ImplicTable ki;
    ImplicTable history;
    while(findRi(sideList))
    {
        ImplicRow r;
        r = sideList.content.back();
        list<tuple<unsigned long, unsigned long, ImplicRow>> R = findR(r, sideList, ki);
        unsigned long long rLiteralsCount = r.countLiterals();
        while(!R.empty())
        {
            unsigned long pos1_2 = get<1>(*R.begin());
            ImplicRow rj;
            rj = move(get<2>(R.front()));
            R.pop_front();
            ImplicRow rk(rj.size());
            for(unsigned long i = 0; i < rk.size(); ++i) {
                rk.set(r.get(i) | rj.get(i), i);
                cout << "(" << (int)r.get(i) << ", " << (int)rj.get(i) << ")";}
            cout << endl;
            rk.set(0, pos1_2);
            ki.content.push_back(rk);
            sweepCovered(ki.content.back());
            sideList.sweepCovered(ki.content.back());
            ki.sweepCovered(ki.content.back());
            if(checkCover(ki.content.back()) || sideList.checkCover(ki.content.back()) || ki.checkCover(ki.content.back()))
            {
                ki.content.pop_back();
                continue;
            }
            if(!rk.get_meta_phase_number(1) && !rk.get_meta_phase_number(2))
                break;
            if(rLiteralsCount < rk.countLiterals() || history.checkCover(ki.content.back()))
            {
                ki.content.pop_back();
                break;
            }

            history.content.push_front(rk);
            r = rk;
            R = findR(r, sideList, ki);
            rLiteralsCount = r.countLiterals();
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
    content.sort();
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
        row.print();
        cout << endl;
    }
}

bool ImplicTable::findRi(ImplicTable& sideList)
{
    while(!content.empty())
    {
        ImplicRow v;
        v = move(content.front());
        content.pop_front();
        sideList.content.push_back(v);
        if((v.get_meta_phase_number(1) || v.get_meta_phase_number(2)) && v.get_meta_phase_number(3))
            return true;
    }
    return false;
}

list<tuple<unsigned long, unsigned long, ImplicRow>> ImplicTable::findR(ImplicRow& r, ImplicTable& sideList, ImplicTable& ki)
{
    list<tuple<unsigned long, unsigned long, ImplicRow>> result;
    auto loop = [&](list<ImplicRow>& l)
    {
        for(auto& rn : l)
        {
            unsigned long zeroPairs = 0;
            unsigned long pos1_2 = 0;
            bool seenOnlyOne1_2 = false;
            bool seenCross3 = false;
            for(unsigned long i = 0; i < rn.size(); ++i)
            {
                if(rn.get(i))
                {
                    if(rn.get(i) == 3)
                    {
                        if(r.get(i) != 3)
                        {
                            seenCross3 = true;
                            break;
                        }
                    }
                    else if((rn.get(i) ^ r.get(i)) == 3)
                    {
                        if(seenOnlyOne1_2)
                        {
                            seenOnlyOne1_2 = false;
                            break;
                        }
                        else
                        {
                            seenOnlyOne1_2 = true;
                            pos1_2 = i;
                        }
                    }
                }
                else if(!(r.get(i)))
                    ++zeroPairs;
            }
            if(seenOnlyOne1_2 && !seenCross3)
                result.push_back(make_tuple(zeroPairs, pos1_2, rn));
        }
    };

    loop(content);
    loop(sideList.content);
    loop(ki.content);
    result.sort(
            [&](tuple<unsigned long, unsigned long, ImplicRow>& first, tuple<unsigned long, unsigned long, ImplicRow>& second)
                {
                    return get<0>(first) > get<0>(second);
                }
    );
    return result;
}
