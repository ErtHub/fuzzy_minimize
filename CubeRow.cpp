//
// Created by hubert on 12.11.17.
//

#include "CubeRow.h"

using namespace std;

CubeRow::CubeRow() : meta_phase_numbers(vector<unsigned long>(4, 0))
{}

CubeRow::CubeRow(vector<unsigned char>& content) : content(content), meta_phase_numbers(vector<unsigned long>(4))
{
    for(auto& i : this->content)
        ++meta_phase_numbers[i];
}

CubeRow::CubeRow(unsigned long size) : content(vector<unsigned char>(size)), meta_phase_numbers(vector<unsigned long>(4))
{
    meta_phase_numbers[0] = size;
}

void CubeRow::set(unsigned char what, unsigned long where)
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

bool CubeRow::covers(const CubeRow& covered) const
{
    if(meta_phase_numbers[3] > covered.meta_phase_numbers[3] || meta_phase_numbers[0] < covered.meta_phase_numbers[0] || countLiterals() > covered.countLiterals())
        return false;
    for(unsigned pos = 0; pos < content.size(); ++pos)
        if(content[pos] && (covered.content[pos] & content[pos]) != content[pos])
            return false;
    return true;
}

unsigned long long CubeRow::countLiterals() const
{
    return meta_phase_numbers[1] + meta_phase_numbers[2] + meta_phase_numbers[3] + meta_phase_numbers[3];
}

unsigned char CubeRow::get(unsigned long where) const
{
    return content[where];
}

unsigned long CubeRow::get_meta_phase_number(unsigned char which) const
{
    return meta_phase_numbers[which];
}

unsigned long CubeRow::size() const
{
    return content.size();
}

CubeRow CubeRow::expand(unsigned long by)
{
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
    twin.set(twin.get(where) ^ (unsigned char)3, where);
    return twin;
}

/*
void CubeRow::print() const
{
    for(auto& i : content)
    {
        cout << (int)i << " ";
    }
    cout << "| ";
    for(auto& i : meta_phase_numbers)
    {
        cout << i << " ";
    }
}*/

ostream& operator<<(ostream& os, const CubeRow& cr)
{
    for(auto& i : cr.content)
        os << (int)i << " ";
    cout << "| ";
    for(auto& i : cr.meta_phase_numbers)
        os << i << " ";
    return os;
}