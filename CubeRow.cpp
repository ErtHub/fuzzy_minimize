#include "CubeRow.h"

using namespace std;

/*A fuzzy function variable can be encoded with four different bit combinations, hence four values in
 * meta_phase_numbers vector*/
SimpleCubeRow::SimpleCubeRow(unsigned long size) : content(CubeRowCont(size)), meta_phase_numbers(vector<unsigned long>(4))
{
    meta_phase_numbers[0] = size;
}

void SimpleCubeRow::set(uint8_t what, unsigned long where)
{
    if(where >= content.size() || what >= meta_phase_numbers.size() || what == content[where])
        return;
    --meta_phase_numbers[content[where]];
    content[where] = what;
    ++meta_phase_numbers[content[where]];
}

int SimpleCubeRow::covers(const SimpleCubeRow& covered) const
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

unsigned long long SimpleCubeRow::countLiterals() const
{
    /*calculate the number of literals the cube represented by the row consists of; each 1 or 2 value mean there is one
     * literal, 3 value means there are two*/
    return meta_phase_numbers[1] + meta_phase_numbers[2] + meta_phase_numbers[3] + meta_phase_numbers[3];
}

uint8_t SimpleCubeRow::get(unsigned long where) const
{
    return content[where];
}

unsigned long SimpleCubeRow::get_meta_phase_number(uint8_t which) const
{
    return meta_phase_numbers[which];
}

unsigned long SimpleCubeRow::size() const
{
    return content.size();
}

SimpleCubeRow SimpleCubeRow::expand(unsigned long by)
{
    //apply Kleen law to expand a row into a sum of two rows - one with a 1 value and the other with 2
    SimpleCubeRow child(*this);
    set(1, by);
    child.set(2, by);
    return child;
}

SimpleCubeRow SimpleCubeRow::phaseSwitchedTwin(unsigned long where) const
{
    SimpleCubeRow twin(*this);
    twin.set(twin.get(where) ^ (uint8_t)3, where);
    return twin;
}

SimpleCubeRow SimpleCubeRow::fuzzyConsensus(const SimpleCubeRow &another, unsigned long pos, int w) const
{
    if(w & VERY_VERBOSE)
        cout << "Calculating consensus: (" << *this << ") psi (" << another << ") = (";
    SimpleCubeRow result(size());
    for(unsigned long i = 0; i < result.size(); ++i)
        result.set(get(i) | another.get(i), i);
    result.set(0, pos);
    if(w & VERY_VERBOSE)
        cout << result << ")" << endl;
    return result;
}

ostream& operator<<(ostream& os, const SimpleCubeRow& cr)
{
    for(auto i : cr.content)
        os << (int)i << " ";
    cout << "| ";
    for(auto i : cr.meta_phase_numbers)
        os << i << " ";
    return os;
}

/*Construct a CondensedCubeRow for a given number of fuzzy function variables; Calculate the number of required unsigned
 * values as number of variables divided by number of variables encoded on one value and rounded up*/
CondensedCubeRow::CondensedCubeRow(unsigned long size) : sizebyvars(size), nwords(size / varsperuint + ((size % varsperuint > 0) ? 1 : 0)), content(CondensedCubeRowCont(nwords)), meta_phase_numbers(vector<unsigned long>(4))
{
    meta_phase_numbers[0] = size;
}

void CondensedCubeRow::set(uint8_t what, unsigned long where)
{
    if(where >= sizebyvars || what >= meta_phase_numbers.size())
        return;
    unsigned long word = where / varsperuint;
    unsigned long offset = (where % varsperuint) * vbits; /*since a variable is encoded on two bits, shift the 0th
    * position in the unsigned value by double the position the variable assumes in it*/
    --meta_phase_numbers[get(where)];
    content[word] &= (UINT_MAX - (3 << offset)); //erase the previous value stored at 'word' and 'offset'
    content[word] |= (what << offset); //set new value given as 'what'
    ++meta_phase_numbers[what];
}

int CondensedCubeRow::covers(const CondensedCubeRow &covered) const
{
    int rowsAreEqual = EQUAL;
    if(meta_phase_numbers[3] > covered.meta_phase_numbers[3] || meta_phase_numbers[0] < covered.meta_phase_numbers[0] || countLiterals() > covered.countLiterals())
        return NO_COVER;

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

unsigned long long CondensedCubeRow::countLiterals() const
{
    return meta_phase_numbers[1] + meta_phase_numbers[2] + meta_phase_numbers[3] + meta_phase_numbers[3];
}

uint8_t CondensedCubeRow::get(unsigned long where) const
{
    /*Pick an unsigned value, where the desired variable is encoded, then extract the code by ANDing it with a mask,
     * where bits at two positions, at which the code is stored, are set to 1; Then, unapply the offset*/
    unsigned long offset = (where % varsperuint) * vbits;
    return (uint8_t)((content[where / varsperuint] & (3 << offset)) >> offset);
}

unsigned long CondensedCubeRow::get_meta_phase_number(uint8_t which) const
{
    return meta_phase_numbers[which];
}

unsigned long CondensedCubeRow::size() const
{
    return sizebyvars;
}

CondensedCubeRow CondensedCubeRow::expand(unsigned long by)
{
    CondensedCubeRow child(*this);
    set(1, by);
    child.set(2, by);
    return child;
}

CondensedCubeRow CondensedCubeRow::phaseSwitchedTwin(unsigned long where) const
{
    CondensedCubeRow twin(*this);
    twin.set(twin.get(where) ^ (uint8_t)3, where);
    return twin;
}

CondensedCubeRow CondensedCubeRow::fuzzyConsensus(const CondensedCubeRow &another, unsigned long pos, int w) const
{
    if(w & VERY_VERBOSE)
        cout << "Calculating consensus: (" << *this << ") psi (" << another << ") = (";
    CondensedCubeRow result(sizebyvars);
    for(unsigned long i = 0; i < result.size(); ++i)
        result.set(get(i) | another.get(i), i);
    result.set(0, pos);
    if(w & VERY_VERBOSE)
        cout << result << ")" << endl;
    return result;
}

ostream& operator<<(ostream& os, const CondensedCubeRow& cr)
{
    for(unsigned long i = 0; i < cr.sizebyvars; ++i)
        os << (int)cr.get(i) << " ";
    cout << "| ";
    for(auto i : cr.meta_phase_numbers)
        os << i << " ";
    return os;
}