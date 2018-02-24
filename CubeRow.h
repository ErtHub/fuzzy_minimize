//
// Created by hubert on 12.11.17.
//

#ifndef FUZZY_MINIMIZE_CUBEROW_H
#define FUZZY_MINIMIZE_CUBEROW_H


#include <iostream>
#include <vector>
#include <list>

enum Covering
{
    NO_COVER = 0,
    NEQUAL,
    EQUAL,
};

using CubeRowCont = std::vector<uint8_t>;

/*this class represents the tetrary-tabular form of a fuzzy expression cube; every position represents a variable of a
 * function and a value is set as 0 if there is no literal representing the variable, 1 if there is a negative literal
 * 2, if there is a positive one and 3 if there are both*/
class CubeRow
{
    CubeRowCont content;//TODO uint8_t : 2?
    //counts of every value in the main row
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit CubeRow();
    explicit CubeRow(CubeRowCont& content);
    explicit CubeRow(unsigned long size);
    void set(uint8_t what, unsigned long where);
    //point all the positions, where the value is 0
    std::list<unsigned long> localize0() const;
    //point all the positions, where the value is 1 or 2
    std::list<unsigned long> localize1_2() const;
    //test if the calling CubeRow object subsumes the one given as 'covered'
    int covers(const CubeRow& covered) const;
    unsigned long long countLiterals() const;
    uint8_t get(unsigned long where) const;
    unsigned long get_meta_phase_number(uint8_t which) const;
    unsigned long size() const;
    CubeRow expand(unsigned long by);
    //used to create a row with a literal designated as 'where' negated
    CubeRow phaseSwitchedTwin(unsigned long where) const;

    friend bool operator<(CubeRow& first, CubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const CubeRow& cr);
};




#endif //FUZZY_MINIMIZE_CUBEROW_H
