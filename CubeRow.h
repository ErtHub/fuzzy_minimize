//
// Created by hubert on 12.11.17.
//

#ifndef FUZZY_MINIMIZE_CUBEROW_H
#define FUZZY_MINIMIZE_CUBEROW_H


#include <iostream>
#include <vector>
#include <list>
#include <climits>

enum Covering
{
    NO_COVER = 0,
    NEQUAL,
    EQUAL,
};

enum Writers
{
    NO_WRITER = 0, VERBOSE = 4, VERY_VERBOSE = 8
};

using CubeRowCont = std::vector<uint8_t>;
using CondensedCubeRowCont = std::vector<unsigned>;

template <uint8_t... Args>
struct CompareImpl;

template <uint8_t First, uint8_t... Args>
struct CompareImpl<First, Args...>
{
    static bool compare(uint8_t i)
    {
        return First == i || CompareImpl<Args...>::compare(i);
    }
};

template <>
struct CompareImpl<>
{
    static bool compare(uint8_t i)
    {
        return false;
    }
};

/*this class represents the tetrary-tabular form of a fuzzy expression cube; every position represents a variable of a
 * function and a value is set as 0 if there is no literal representing the variable, 1 if there is a negative literal
 * 2, if there is a positive one and 3 if there are both*/
/*class CubeRow
{
    CubeRowCont content;
    //counts of every value in the main row
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit CubeRow();
    explicit CubeRow(CubeRowCont& content);
    explicit CubeRow(unsigned long size);
    void set(uint8_t what, unsigned long where);

    template <uint8_t... Args> std::list<unsigned long> localize() const
    {
        std::list<unsigned long> positions;
        for(unsigned long i = 0; i < content.size(); ++i)
            if(CompareImpl<Args...>::compare(content[i]))
                positions.push_back(i);
        return positions;
    };

    //test if the calling CubeRow object subsumes the one given as 'covered'
    int covers(const CubeRow& covered) const;
    unsigned long long countLiterals() const;
    uint8_t get(unsigned long where) const;
    unsigned long get_meta_phase_number(uint8_t which) const;
    unsigned long size() const;
    CubeRow expand(unsigned long by);
    //used to create a row with a literal designated as 'where' negated
    CubeRow phaseSwitchedTwin(unsigned long where) const;
    CubeRow fuzzyConsensus(const CubeRow& another, unsigned long pos, int w = 0) const;

    friend bool operator<(CubeRow& first, CubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const CubeRow& cr);
};*/

class CondensedCubeRow
{
    unsigned long sizebyvars;
    static const int vbits = 2;
    static const int ubitpairs = (sizeof(unsigned) * CHAR_BIT) / vbits;
    unsigned long nwords;

    CondensedCubeRowCont content;
    //counts of every value in the main row
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit CondensedCubeRow(unsigned long size = 0);
    void set(uint8_t what, unsigned long where);

    template <uint8_t... Args> std::list<unsigned long> localize() const
    {
        std::list<unsigned long> positions;
        for(unsigned long i = 0; i < sizebyvars; ++i)
            if(CompareImpl<Args...>::compare(get(i)))
                positions.push_back(i);
        return positions;
    };

    //test if the calling CubeRow object subsumes the one given as 'covered'
    int covers(const CondensedCubeRow& covered) const;
    unsigned long long countLiterals() const;
    uint8_t get(unsigned long where) const;
    unsigned long get_meta_phase_number(uint8_t which) const;
    unsigned long size() const;
    CondensedCubeRow expand(unsigned long by);
    //used to create a row with a literal designated as 'where' negated
    CondensedCubeRow phaseSwitchedTwin(unsigned long where) const;
    CondensedCubeRow fuzzyConsensus(const CondensedCubeRow& another, unsigned long pos, int w = 0) const;

    friend bool operator<(CondensedCubeRow& first, CondensedCubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const CondensedCubeRow& cr);
};

using CubeRow = CondensedCubeRow;


#endif //FUZZY_MINIMIZE_CUBEROW_H
