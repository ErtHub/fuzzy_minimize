//
// Created by hubert on 23.07.17.
//

#include "CubeTable.h"

using namespace std;

CubeTable::CubeTable(const CubeTableCont &content, int w) : content(content), write(w)
{}

CubeTable::CubeTable(const FuzzyFunction &func, int w) : content(func.tabulate()), write(w)
{}

void CubeTable::sweepCovered(CubeTable& another, int sweepEqual)
{
    //sweepCovered();
    //another.sweepCovered();
    for(auto& i : content)
        another.sweepCovered(i, sweepEqual);
    for(auto& i : another.content)
        sweepCovered(i, sweepEqual);
}

void CubeTable::sweepCovered(int sweepEqual)
{
    for(auto& i : content)
        sweepCovered(i, sweepEqual);
}

bool CubeTable::sweepCovered(const CubeRow& i, int sweepEqual)
{
    int equality = 0;
    int equalitiesSoFar = 0;
    for(auto k = content.begin(); k != content.end();)
    {
        if(addressof(*k) != addressof(i) && ((equality = i.covers(*k)) & sweepEqual))
            k = content.erase(k);
        else
            ++k;
        equalitiesSoFar |= equality;
    }
    return (bool)(equalitiesSoFar & EQUAL);
}

void CubeTable::append(const CubeRow& item)
{
    content.push_back(item);
}

bool CubeTable::checkCover(const CubeRow& covered, int checkEqual) const
{
    /*for(auto& i : content)
        if((addressof(i) != addressof(covered)) && i.covers(covered))
            return true;
    return false;*/
    return any_of(content.begin(), content.end(), [&](const CubeRow& c){ return (addressof(c) != addressof(covered)) && (c.covers(covered) & checkEqual); });
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
            //for every possible pair of rows find the 3 value in at least one of them...
            if(i->get_meta_phase_number(3) || j->get_meta_phase_number(3))
            {
                for(auto &pos : positions1_2)
                {
                    //...and for every 1-2 value pair create one fuzzy consensus row...
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
                        //...and append it to the K1 table
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
        //for every row, if it's complementary and can be Kleen-expanded...
        if(!cube.get_meta_phase_number(0) || !cube.get_meta_phase_number(3))
        {
            subset.push_back(move(cube));
            continue;
        }
        list<unsigned long> positions0 = move(cube.localize0());
        auto divisionPos = positions0.begin();
        auto pos0End = positions0.end();
        CubeRow halfCube1 = cube;
        //...expand it...
        CubeRow halfCube2 = move(halfCube1.expand(*divisionPos));
        /*...until all expansion-derived rows are subsumed by one another row in the function; if a row is not subsumed
         * and cannot be expanded, it cannot be omitted*/
        if(!(recursiveCover(cube, halfCube1, ++divisionPos, pos0End, subset) && recursiveCover(cube, halfCube2, divisionPos, pos0End, subset)))
            subset.push_back(move(cube));
    }
    content = move(subset);
}

CubeTable CubeTable::separateEssentials()
{
    CubeTableCont essentials;

    for(auto i = content.begin(); i != content.end();)
    {
        //for every row, if it's complementary and can be Kleen-expanded...
        if(!i->get_meta_phase_number(0) || !i->get_meta_phase_number(3))
        {
            essentials.push_back(move(*i));
            i = content.erase(i);
            continue;
        }
        list<unsigned long> positions0 = move(i->localize0());
        auto divisionPos = positions0.begin();
        auto pos0End = positions0.end();
        CubeRow halfCube1 = *i;
        //...expand it...
        CubeRow halfCube2 = move(halfCube1.expand(*divisionPos));
        /*...until all expansion-derived rows are subsumed by one another row in the function; if a row is not subsumed
         * and cannot be expanded, the original cube is an essential prime implicant*/
        if(!(recursiveCover(*i, halfCube1, ++divisionPos, pos0End, essentials) && recursiveCover(*i, halfCube2, divisionPos, pos0End, essentials)))
        {
            essentials.push_back(move(*i));
            i = content.erase(i);
        }
        else
            ++i;
    }
    return CubeTable(essentials);
}

bool CubeTable::recursiveCover(CubeRow& original, CubeRow& cube, list<unsigned long>::iterator pos0, list<unsigned long>::iterator& pos0End, const CubeTableCont& secondList) const
{
    /*for(auto& i : content)
        if(i.covers(cube))
            return true;
    for(auto& i : secondList)
        if(i.covers(cube))
            return true;*/
    function<bool(const CubeRow&)> checkFun = [&](const CubeRow& c) { return addressof(c) != addressof(original) && c.covers(cube); };
    if(any_of(content.begin(), content.end(), checkFun) || any_of(secondList.begin(), secondList.end(), checkFun))
        return true;
    if(pos0 == pos0End)
        return false;
    CubeRow cube2 = move(cube.expand(*pos0));

    return recursiveCover(original, cube, ++pos0, pos0End, secondList) && recursiveCover(original, cube2, pos0, pos0End, secondList);
}

bool CubeTable::omissionAllowed(CubeRow &cube, unsigned long position) const
{
    //M. Mukaidono - "Fuzzy logical functions and their minimal and irredundant form"
    CubeRow twin = move(cube.phaseSwitchedTwin(position));
    return (bool)checkCover(twin);
}

bool CubeTable::omissionAllowedRecursively(CubeRow &cube, unsigned long position,
                                             std::list<unsigned long>::iterator pos0,
                                             std::list<unsigned long>::iterator &pos0End) const
{
    CubeRow twin1 = move(cube.phaseSwitchedTwin(position));
    CubeRow twin2 = twin1.expand(*pos0);
    return recursiveCover(cube, twin1, ++pos0, pos0End) && recursiveCover(cube, twin2, pos0, pos0End);
}

void CubeTable::expandAndFilter(CubeRow &cube, std::list<unsigned long>::iterator pos0,
                                std::list<unsigned long>::iterator &pos0End, CubeTableCont &target) const
{
    if(checkCover(cube))
        return;
    if(pos0 == pos0End)
    {
        target.push_back(move(cube));
        return;
    }
    CubeRow cube2 = move(cube.expand(*pos0));
    expandAndFilter(cube, ++pos0, pos0End, target);
    expandAndFilter(cube2, pos0, pos0End, target);

}

void CubeTable::minimizeExact()
{
    CubeTable k1;
    bool wasEmpty = true;
    do
    {
        k1 = move(generateK1());
#ifdef SORT_TABLES
        k1.content.sort();
#endif
        k1.sweepCovered();
        sweepCovered(k1);
        wasEmpty = k1.empty();
        merge(k1);
        if(write)
            cout << *this << "----------------" << endl;
        //sweepCovered();
    } while(!wasEmpty);//generate fuzzy consensus and append it to K table until K table's content changes
}

void CubeTable::minimizeHeuristic()
{
#ifdef SORT_TABLES
    content.sort();
#endif
    sweepCovered();
    unsigned long originalSize = content.size();
    CubeTable sideList;
    CubeTable ki;
//    CubeTable history;
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

            bool rowWasRepeated = sweepCovered(ki.content.back(), NEQUAL);
            rowWasRepeated |= sideList.sweepCovered(ki.content.back(), NEQUAL);
            rowWasRepeated |= ki.sweepCovered(ki.content.back(), NEQUAL);

            if(checkCover(ki.content.back(), NEQUAL) || sideList.checkCover(ki.content.back(), NEQUAL) || ki.checkCover(ki.content.back(), NEQUAL))
            {
                ki.content.pop_back();
                continue;
            }
            if(!rk.get_meta_phase_number(1) && !rk.get_meta_phase_number(2))
                break;
            if(rLiteralsCount < rk.countLiterals() || rowWasRepeated)
            {
                ki.content.pop_back();
                break;
            }

//            history.content.push_front(rk);
            r = rk;
            R = findR(r, sideList, ki);
            rLiteralsCount = r.countLiterals();
            if(write)
                cout << *this << endl << sideList << endl << ki << "----------------" << endl;
        }
    }
    merge(sideList);
    if(content.size() < originalSize)
        merge(ki);
//    content.sort();
//    separateEssentials();
}

void CubeTable::minimizeMukaidono()
{
#ifdef SORT_TABLES
    content.sort();
#endif
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

unsigned long CubeTable::size() const
{
    return content.size();
}

void CubeTable::merge(CubeTable &another)
{
#ifdef SORT_TABLES
    content.merge(another.content);
#else
    content.splice(content.end(), another.content);
#endif
}

void CubeTable::sort()
{
    content.sort();
}

void CubeTable::clear()
{
    content.clear();
}

CubeRow CubeTable::pop_front()
{
    if(empty())
        return CubeRow();
    CubeRow result = move(content.front());
    content.pop_front();
    return result;
}

/*void CubeTable::print()
{
    for(auto& row : content)
    {
        row.print();
        cout << endl;
    }
}*/

ostream& operator<<(ostream& os, const CubeTable& ct)
{
    for(auto& row : ct.content)
//    {
        os << row << endl;
//        os << endl;
//    }
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
    auto loop = [&](CubeTableCont& l)
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
            /*for a given r row, create an r_n set of rows; for each r_n row, count the 0 values, localize the 1-2 value
             * pair with the r row and sort them by the 0 values count*/
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
}//TODO zrobic funkcje rekurencyjne iteracyjnie

FunctionBody CubeTable::redeem(const VarTable& tab) const
{
    FunctionBody result;

    vector<string> symbRow(tab.size(), "");

    /*std::unordered_map scrambles the order of variable names which can make the fuzzy expression harder to read;
     * this action creates the ordered symbol row in order to revert this alteration*/
    for(auto& i : tab)
        symbRow[i.second] = i.first;

    for(auto& row : content)
    {
        CubeCont partialResult;
        for(unsigned long i = 0; i < symbRow.size(); ++i)
        {
            if(row.get(i))
            {
                if(row.get(i) & 2)
                    partialResult.emplace_back(SymbInstance(symbRow[i], false));
                if(row.get(i) & 1)
                    partialResult.emplace_back(SymbInstance(symbRow[i], true));
            }
        }
        result.emplace_back(Cube(partialResult));
    }

    return result;
}

CubeTable CubeTable::crossProduct(const CubeTable &another) const
{
    CubeTable result;
    for(auto& i : content)
    {
        for(auto& j : another.content)
        {
            CubeRow toInsert(i.size() > j.size() ? i.size() : j.size());
            for(unsigned long pos = 0; pos < i.size() && pos < j.size(); ++pos)
                toInsert.set(i.get(pos) | j.get(pos), pos);
//            cout << toInsert << "hubert" << endl;
            result.append(toInsert);
        }
    }
    result.sweepCovered();
    return result;
}

CubeTableCont CubeTable::findUncoveredCompletes(const CubeTable &covering) const
{
    CubeTableCont result;

    for(auto& i : content)
    {
        if(i.get_meta_phase_number(3))
        {
            if(!i.get_meta_phase_number(0))
            {
                if(!covering.checkCover(i))
                    result.push_back(i);
                continue;
            }
            list<unsigned long> positions0 = move(i.localize0());
            auto divisionPos = positions0.begin();
            auto pos0End = positions0.end();
            CubeRow cube1 = i;
            CubeRow cube2 = cube1.expand(*divisionPos);
            covering.expandAndFilter(cube1, ++divisionPos, pos0End, result);
            covering.expandAndFilter(cube2, divisionPos, pos0End, result);
        }
    }
    return result;
}