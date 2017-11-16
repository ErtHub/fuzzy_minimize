//
// Created by hubert on 12.11.17.
//

#ifndef FUZZY_MINIMIZE_IMPLICROW_H
#define FUZZY_MINIMIZE_IMPLICROW_H


#include <iostream>
#include <vector>
#include <list>

class ImplicRow
{
    std::vector<unsigned char> content;
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit ImplicRow();
    explicit ImplicRow(std::vector<unsigned char>& content);
    explicit ImplicRow(unsigned long size);
    void set(unsigned char what, unsigned long where);
    std::list<unsigned long> localize0() const;
    std::list<unsigned long> localize1_2() const;
    bool covers(const ImplicRow& covered) const;
    unsigned long long countLiterals() const;
    unsigned char get(unsigned long where) const;
    unsigned long get_meta_phase_number(unsigned char which) const;
    unsigned long size() const;
    ImplicRow expand(unsigned long by);
    ImplicRow phaseSwitchedTwin(unsigned long where) const;
    void print() const;
    friend bool operator<(ImplicRow& first, ImplicRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
};




#endif //FUZZY_MINIMIZE_IMPLICROW_H
