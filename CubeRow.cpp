//
// Created by hubert on 12.11.17.
//

#include "CubeRow.h"

using namespace std;

CubeRow::CubeRow() : meta_phase_numbers(vector<unsigned long>(4, 0))
{}

CubeRow::CubeRow(CubeRowCont& content) : content(content), meta_phase_numbers(vector<unsigned long>(4))
{
    for(auto i : this->content)
        ++meta_phase_numbers[i];
}

CubeRow::CubeRow(unsigned long size) : content(CubeRowCont(size)), meta_phase_numbers(vector<unsigned long>(4))
{
    meta_phase_numbers[0] = size;
}

void CubeRow::set(uint8_t what, unsigned long where)
{
    if(where >= content.size() || what >= meta_phase_numbers.size() || what == content[where])
        return;
    --meta_phase_numbers[content[where]];
    content[where] = what;
    ++meta_phase_numbers[content[where]];
}

list<unsigned long> CubeRow::localize0() const
{
    if(!meta_phase_numbers[0])
        return list<unsigned long>();
    list<unsigned long> positions0;
    for(unsigned long i = 0; i < content.size(); ++i)
        if(!get(i))
            positions0.push_back(i);
    return positions0;
}

list<unsigned long> CubeRow::localize1_2() const
{
    if(!meta_phase_numbers[1] && !meta_phase_numbers[2])
        return list<unsigned long>();
    list<unsigned long> positions1_2;
    for(unsigned long i = 0; i < content.size(); ++i)
        if(content[i] == 1 || content[i] == 2)
            positions1_2.push_back(i);
    return positions1_2;
}

int CubeRow::covers(const CubeRow& covered) const
{
    int rowsAreEqual = EQUAL;
    if(meta_phase_numbers[3] > covered.meta_phase_numbers[3] || meta_phase_numbers[0] < covered.meta_phase_numbers[0] || countLiterals() > covered.countLiterals())
        return NO_COVER;
    /*the c1 cube subsumes the c2 cube if c2 contains all the literals of c1; the numbers put in these CubeRow
     * objects are in their binary representation exactly the switches designating if a literal exists in the cube -
     * 01 for a negative literal and 10 for a positive one; therefore, the test for the relationship of subsuming can be
     * performed as the equality of the subsuming row and the bitwise AND of the subsuming and the subsumed row*/
    for(unsigned pos = 0; pos < content.size(); ++pos)
    {
        if(content[pos] != covered.content[pos])
        {
            rowsAreEqual = NEQUAL;
            if(content[pos] && (covered.content[pos] & content[pos]) != content[pos])
                return NO_COVER;
        }
    }
    return rowsAreEqual;
}

unsigned long long CubeRow::countLiterals() const
{
    /*calculate the number of literals the cube represented by the row consists of; each 1 or 2 value mean there is one
     * literal, 3 value means there are two*/
    return meta_phase_numbers[1] + meta_phase_numbers[2] + meta_phase_numbers[3] + meta_phase_numbers[3];
}

uint8_t CubeRow::get(unsigned long where) const
{
    return content[where];
}

unsigned long CubeRow::get_meta_phase_number(uint8_t which) const
{
    return meta_phase_numbers[which];
}

unsigned long CubeRow::size() const
{
    return content.size();
}

CubeRow CubeRow::expand(unsigned long by)
{
    //apply Kleen law to expand a row into a sum of two rows - one with a 1 value and the other with 2
    if(by >= size() || content[by])
        return CubeRow();
    CubeRow child(*this);
    set(1, by);
    child.set(2, by);
    return child;
}

CubeRow CubeRow::phaseSwitchedTwin(unsigned long where) const
{
    CubeRow twin(*this);
    twin.set(twin.get(where) ^ (uint8_t)3, where);
    return twin;
}

CubeRow CubeRow::fuzzyConsensus(const CubeRow &another, unsigned long pos, int w) const
{
    CubeRow result(size());
    for(unsigned long i = 0; i < result.size(); ++i)
    {
        result.set(get(i) | another.get(i), i);
        if(w & VERY_VERBOSE)
            cout << "(" << (int)get(i) << ", " << (int)another.get(i) << ")";
    }
    if(w & VERY_VERBOSE)
        cout << endl;
    result.set(0, pos);
    return result;
}

ostream& operator<<(ostream& os, const CubeRow& cr)
{
    for(auto i : cr.content)
        os << (int)i << " ";
    cout << "| ";
    for(auto i : cr.meta_phase_numbers)
        os << i << " ";
    return os;
}