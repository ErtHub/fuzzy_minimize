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

/*The three templates below implement a function that tests if a given number is equal to any of the ones specified in
 * template parameters*/
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
class SimpleCubeRow
{
    CubeRowCont content;
    //counts of every value in the main row
    std::vector<unsigned long> meta_phase_numbers;

public:
    explicit SimpleCubeRow(unsigned long size = 0);
    void set(uint8_t what, unsigned long where);

    /*Assemble and return a list of positions where a value is equal to any of the numbers specified in template
     * parameters*/
    template <uint8_t... Args> std::list<unsigned long> localize() const
    {
        std::list<unsigned long> positions;
        for(unsigned long i = 0; i < content.size(); ++i)
            if(CompareImpl<Args...>::compare(content[i]))
                positions.push_back(i);
        return positions;
    };

    //test if the calling CubeRow object subsumes the one given as 'covered'
    int covers(const SimpleCubeRow& covered) const;
    unsigned long long countLiterals() const;
    uint8_t get(unsigned long where) const;
    unsigned long get_meta_phase_number(uint8_t which) const;
    unsigned long size() const;
    /*Out of the CubeRow, make two CubeRows representing cubes being a result of Kleen expansion of the cube represented
     * by the original CubeRow, by the variable determined with the 'by' value. The CubeRow with the positive literal is
     * returned from the function and one with the negative literal is substituted as the calling object*/
    SimpleCubeRow expand(unsigned long by);
    //used to create a row with a literal designated as 'where' negated
    SimpleCubeRow phaseSwitchedTwin(unsigned long where) const;
    SimpleCubeRow fuzzyConsensus(const SimpleCubeRow& another, unsigned long pos, int w = 0) const;

    friend bool operator<(SimpleCubeRow& first, SimpleCubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const SimpleCubeRow& cr);
};

/*Memory-saving version of SimpleCubeRow - stores up to 16 FuzzyFunction variable in every single unsigned int value;
 * To switch between CubeRow versions, comment/uncomment the "add_definitions(-DCONDENSE_CUBEROWS)" line in
 * CMakeLists.txt*/
class CondensedCubeRow
{
    unsigned long sizebyvars; //number of variables that apply for the original fuzzy function
    static const int vbits = 2; //number of bits required to encode literals of a single variable
    static const int varsperuint = (sizeof(unsigned) * CHAR_BIT) / vbits; //number of variables that fit on an unsigned
    unsigned long nwords; //number of unsigned values required to store all given variables

    CondensedCubeRowCont content;
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

    int covers(const CondensedCubeRow& covered) const;
    unsigned long long countLiterals() const;
    uint8_t get(unsigned long where) const;
    unsigned long get_meta_phase_number(uint8_t which) const;
    unsigned long size() const;
    CondensedCubeRow expand(unsigned long by);
    CondensedCubeRow phaseSwitchedTwin(unsigned long where) const;
    CondensedCubeRow fuzzyConsensus(const CondensedCubeRow& another, unsigned long pos, int w = 0) const;

    friend bool operator<(CondensedCubeRow& first, CondensedCubeRow& second)
    {
        return first.countLiterals() < second.countLiterals();
    }
    friend std::ostream& operator<<(std::ostream& os, const CondensedCubeRow& cr);
};

#ifdef CONDENSE_CUBEROWS
using CubeRow = CondensedCubeRow;
#else
using CubeRow = SimpleCubeRow;
#endif


#endif //FUZZY_MINIMIZE_CUBEROW_H
