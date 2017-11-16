//
// Created by hubert on 12.11.17.
//

#include "ImplicRow.h"

using namespace std;

ImplicRow::ImplicRow() : meta_phase_numbers(vector<unsigned long>(4, 0))
{}

ImplicRow::ImplicRow(vector<unsigned char>& content) : content(content), meta_phase_numbers(vector<unsigned long>(4))
{
    for(auto& i : this->content)
        ++meta_phase_numbers[i];
}

ImplicRow::ImplicRow(unsigned long size) : content(vector<unsigned char>(size)), meta_phase_numbers(vector<unsigned long>(4))
{
    meta_phase_numbers[0] = size;
}

void ImplicRow::set(unsigned char what, unsigned long where)
{
    if(where >= content.size() || what >= meta_phase_numbers.size() || what == content[where])
        return;
    --meta_phase_numbers[content[where]];
    content[where] = what;
    ++meta_phase_numbers[content[where]];
}

list<unsigned long> ImplicRow::localize0() const
{
    if(!meta_phase_numbers[0])
        return list<unsigned long>();
    list<unsigned long> positions0;
    for(unsigned long i = 0; i < content.size(); ++i)
        if(!get(i))
            positions0.push_back(i);
    return positions0;
}

list<unsigned long> ImplicRow::localize1_2() const
{
    if(!meta_phase_numbers[1] && !meta_phase_numbers[2])
        return list<unsigned long>();
    list<unsigned long> positions1_2;
    for(unsigned long i = 0; i < content.size(); ++i)
        if(content[i] == 1 || content[i] == 2)
            positions1_2.push_back(i);
    return positions1_2;
}

bool ImplicRow::covers(const ImplicRow& covered) const
{
    if(meta_phase_numbers[3] > covered.meta_phase_numbers[3] || meta_phase_numbers[0] < covered.meta_phase_numbers[0] || countLiterals() > covered.countLiterals())
        return false;
    for(unsigned pos = 0; pos < content.size(); ++pos)
        if(content[pos] && (covered.content[pos] & content[pos]) != content[pos])
            return false;
    return true;
}

unsigned long long ImplicRow::countLiterals() const
{
    return meta_phase_numbers[1] + meta_phase_numbers[2] + meta_phase_numbers[3] + meta_phase_numbers[3];
}

unsigned char ImplicRow::get(unsigned long where) const
{
    return content[where];
}

unsigned long ImplicRow::get_meta_phase_number(unsigned char which) const
{
    return meta_phase_numbers[which];
}

unsigned long ImplicRow::size() const
{
    return content.size();
}

ImplicRow ImplicRow::expand(unsigned long by)
{
    if(by >= size() || content[by])
        return ImplicRow();
    ImplicRow child(*this);
    set(1, by);
    child.set(2, by);
    return child;
}

ImplicRow ImplicRow::phaseSwitchedTwin(unsigned long where) const
{
    ImplicRow twin(*this);
    twin.set(twin.get(where) ^ (unsigned char)3, where);
    return twin;
}

void ImplicRow::print() const
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
}