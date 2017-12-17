//
// Created by hubert on 23.07.17.
//

#include "CubeTable.h"

using namespace std;

CubeTable::CubeTable(const std::list<CubeRow> &content, int w) : content(content), write(w)
{}

CubeTable::CubeTable(const FuzzyFunction &func, int w) : content(func.tabulate()), write(w)
{}

void CubeTable::sweepCovered(CubeTable& another)
{
    sweepCovered();
    another.sweepCovered();
    for(auto& i : content)
        another.sweepCovered(i);
    for(auto& i : another.content)
        sweepCovered(i);
}

void CubeTable::sweepCovered()
{
    for(auto& i : content)
        sweepCovered(i);
}

void CubeTable::sweepCovered(const CubeRow& i)
{
    for(auto k = content.begin(); k != content.end();)
    {
        if(addressof(*k) != addressof(i) && i.covers(*k))
            k = content.erase(k);
        else
            ++k;
    }
}

void CubeTable::append(const CubeRow& item)
{
    content.push_back(item);
}

bool CubeTable::checkCover(const CubeRow& covered) const
{
    for(auto& i : content)
        if(addressof(i) != addressof(covered) && i.covers(covered))
            return true;
    return false;
}

CubeTable CubeTable::generateK1() const
{
    CubeTable result;

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
                        CubeRow v(j->size());
                        for(unsigned long p = 0; p < v.size(); ++p)
                        {
                            v.set(i->get(p) | j->get(p), p);
                            if(write & 2)
                                cout << "(" << (int)i->get(p) << ", " << (int)j->get(p) << ")";
                        }
                        if(write & 2)
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

void CubeTable::chooseCoveringSubset()
{
    list<CubeRow> subset;
    while(!content.empty())
    {
        CubeRow cube = move(content.front());
        content.pop_front();
        if(!cube.get_meta_phase_number(0) || !cube.get_meta_phase_number(3))
        {
            subset.push_back(move(cube));
            continue;
        }
        list<unsigned long> positions0 = move(cube.localize0());
        auto divisionPos = positions0.begin();
        auto pos0End = positions0.end();
        CubeRow halfCube1 = cube;
        CubeRow halfCube2 = move(halfCube1.expand(*divisionPos));
        if(!(recursiveCover(halfCube1, subset, ++divisionPos, pos0End) && recursiveCover(halfCube2, subset, divisionPos, pos0End)))
            subset.push_back(move(cube));
    }
    content = move(subset);
}

bool CubeTable::recursiveCover(CubeRow& cube, const list<CubeRow>& subset, list<unsigned long>::iterator pos0, list<unsigned long>::iterator& pos0End) const
{
    for(auto& i : content)
        if(i.covers(cube))
            return true;
    for(auto& i : subset)
        if(i.covers(cube))
            return true;
    if(pos0 == pos0End)
        return false;
    CubeRow Cube2 = move(cube.expand(*pos0));

    return recursiveCover(cube, subset, ++pos0, pos0End) && recursiveCover(Cube2, subset, pos0, pos0End);
}

bool CubeTable::omissionAllowed(CubeRow &cube, unsigned long position) const
{
    CubeRow twin = move(cube.phaseSwitchedTwin(position));
    return checkCover(twin);
}

bool CubeTable::omissionAllowedRecursively(CubeRow &cube, unsigned long position,
                                             std::list<unsigned long>::iterator pos0,
                                             std::list<unsigned long>::iterator &pos0End) const
{
    CubeRow twin1 = move(cube.phaseSwitchedTwin(position));
    CubeRow twin2 = twin1.expand(*pos0);
    return recursiveCover(twin1, list<CubeRow>(), ++pos0, pos0End) && recursiveCover(twin2, list<CubeRow>(), pos0, pos0End);
}

void CubeTable::minimizeExact()
{
    content.sort();
    sweepCovered();
    CubeTable k1;
    bool wasEmpty = true;
    do
    {
        k1 = move(generateK1());
        k1.content.sort();
        sweepCovered(k1);
        wasEmpty = k1.empty();
        merge(k1);
        if(write)
            cout << *this << "----------------" << endl;
        //sweepCovered();
    } while(!wasEmpty);
    chooseCoveringSubset();
}

void CubeTable::minimizeHeuristic()
{
    content.sort();
    sweepCovered();
    CubeTable sideList;
    CubeTable ki;
    CubeTable history;
    while(findRi(sideList))
    {
        CubeRow r;
        r = sideList.content.back();
        list<tuple<unsigned long, unsigned long, CubeRow>> R = findR(r, sideList, ki);
        unsigned long long rLiteralsCount = r.countLiterals();
        while(!R.empty())
        {
            unsigned long pos1_2 = get<1>(*R.begin());
            CubeRow rj;
            rj = move(get<2>(R.front()));
            R.pop_front();
            CubeRow rk(rj.size());
            for(unsigned long i = 0; i < rk.size(); ++i)
            {
                rk.set(r.get(i) | rj.get(i), i);
                if(write & 2)
                    cout << "(" << (int)r.get(i) << ", " << (int)rj.get(i) << ")";
            }
            if(write & 2)
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
            if(write)
                cout << *this << endl << sideList << endl << ki << "----------------" << endl;
        }
    }
    merge(sideList);
    merge(ki);
//    content.sort();
//    chooseCoveringSubset();
}

void CubeTable::minimizeMukaidono()
{
    content.sort();
    sweepCovered();
    for(auto& i : content)
    {
        if(!i.get_meta_phase_number(3))
            continue;
        if(write)
            cout << *this << endl << "---------------" << endl;
        auto pos1_2List = i.localize1_2();
        auto pos1_2 = pos1_2List.begin();
        while(pos1_2 != pos1_2List.end())
        {
            if(omissionAllowed(i, *pos1_2))
            {
                i.set(0, *pos1_2);
                pos1_2 = pos1_2List.erase(pos1_2);
            }
            else
                ++pos1_2;
        }
        if(pos1_2List.empty())
        {
            sweepCovered(i);
            continue;
        }
        auto pos0List = i.localize0();
        auto pos0 = pos0List.begin();
        auto pos0End = pos0List.end();
        pos1_2 = pos1_2List.begin();
        while(pos1_2 != pos1_2List.end())
        {
            if(omissionAllowedRecursively(i, *pos1_2, pos0, pos0End))
            {
                i.set(0, *pos1_2);
                pos1_2 = pos1_2List.erase(pos1_2);
            }
            else
                ++pos1_2;
        }
        sweepCovered(i);
    }
}
bool CubeTable::empty() const
{
    return content.empty();
}

void CubeTable::merge(CubeTable &another)
{
    content.merge(another.content);
}

/*void CubeTable::print()
{
    for(auto& row : content)
    {
        row.print();
        cout << endl;
    }
}*/
//TODO prywatyzacja xD
ostream& operator<<(ostream& os, const CubeTable& ct)
{
    for(auto& row : ct.content)
    {
        os << row;
        os << endl;
    }
    return os;
}

bool CubeTable::findRi(CubeTable& sideList)
{
    while(!content.empty())
    {
        CubeRow v;
        v = move(content.front());
        content.pop_front();
        sideList.content.push_back(v);
        if((v.get_meta_phase_number(1) || v.get_meta_phase_number(2)) && v.get_meta_phase_number(3))
            return true;
    }
    return false;
}

list<tuple<unsigned long, unsigned long, CubeRow>> CubeTable::findR(CubeRow& r, CubeTable& sideList, CubeTable& ki)
{
    list<tuple<unsigned long, unsigned long, CubeRow>> result;
    auto loop = [&](list<CubeRow>& l)
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
            [&](tuple<unsigned long, unsigned long, CubeRow>& first, tuple<unsigned long, unsigned long, CubeRow>& second)
                {
                    return get<0>(first) > get<0>(second);
                }
    );
    return result;
}//TODO optymizacja, std::algorithm?, zrobic funkcje rekurencyjne iteracyjnie

list<Cube> CubeTable::redeem(const unordered_map<string, unsigned long>& tab) const
{
    list<Cube> result;

    for(auto& row : content)
    {
        list<SymbInstance> partialResult;
        for(auto& symb : tab)
        {
            if(row.get(symb.second))
            {
                if(row.get(symb.second) & 2)
                    partialResult.emplace_back(SymbInstance(symb.first, false));
                if(row.get(symb.second) & 1)
                    partialResult.emplace_back(SymbInstance(symb.first, true));
            }
        }
        result.emplace_back(Cube(partialResult));
    }

    return result;
}