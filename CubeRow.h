//
// Created by hubert on 12.11.17.
//

#ifndef FUZZY_MINIMIZE_CUBEROW_H
#define FUZZY_MINIMIZE_CUBEROW_H


#include <iostream>
#include <vector>
#include <list>

class CubeRow
{
    std::vector<unsigned char> content;
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit CubeRow();
    explicit CubeRow(std::vector<unsigned char>& content);
    explicit CubeRow(unsigned long size);
    void set(unsigned char what, unsigned long where);
    std::list<unsigned long> localize0() const;
    std::list<unsigned long> localize1_2() const;
    bool covers(const CubeRow& covered) const;
    unsigned long long countLiterals() const;
    unsigned char get(unsigned long where) const;
    unsigned long get_meta_phase_number(unsigned char which) const;
    unsigned long size() const;
    CubeRow expand(unsigned long by);
    CubeRow phaseSwitchedTwin(unsigned long where) const;

    friend bool operator<(CubeRow& first, CubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const CubeRow& cr);
};




#endif //FUZZY_MINIMIZE_CUBEROW_H
