//
// Created by hubert on 20.12.17.
//

#include "FunctionGenerator.h"

using namespace std;

FuzzyFunction FunctionGenerator::generateOne(unsigned long vCount, unsigned long cCount)
{

    cout << "3" << endl;
    for(unsigned long i = 0; i < vCount; ++i)
        varTable.insert(make_pair(("z" + to_string(i)), i));

    CubeTable tetraryTable;

    random_device rd;
    mt19937 rng(rd());

    for(unsigned long i = 0; i < cCount; ++i)
    {
        CubeRow row(vCount);


        /*every function is generated randomly by choosing at random the times a literal should be inserted into every
         * cube the function is supposed to consist of; the minimal count is 1, the maximal is double the number of
         * function's variables; then, the generator chooses at random the variable and its negativeness and the
         * respective literal is put into cube what is repeated as many times as chosen before*/
        uniform_int_distribution<unsigned long> uni(1, vCount * 2);
        unsigned long timesRepeat = uni(rng);
        uniform_int_distribution<unsigned long> tweakerWhere(0, vCount - 1);
        uniform_int_distribution<uint8_t> tweakerWhat(1, 2);//TODO moze to powinno byc 1, 3?

        for(unsigned long v = 0; v < timesRepeat; ++v)
        {
            unsigned long where = tweakerWhere(rng);
            row.set(row.get(where) | tweakerWhat(rng), where);
        }

        tetraryTable.append(row);
    }

    cout << tetraryTable << endl;

    return FuzzyFunction(varTable, tetraryTable);
}

void FunctionGenerator::generate()
{
    cout << "2" << endl;
    if(varCount == 0 || cubeCount == 0)
        return;
    for(unsigned long i = 0; i < funCount; ++i)
        result.emplace_back(make_pair("F" + to_string(i), generateOne(varCount, cubeCount)));
}

bool FunctionGenerator::writeToFile(const string& fn) const
{
    vector<string> symbRow(varTable.size(), "");

    /*descramble the symbol order*/
    for(auto& i : varTable)
        symbRow[i.second] = i.first;

    cout << "1" << endl;
    ofstream ostr;

    ostr.open(fn.c_str());
    if(!ostr)
        return false;

    ostr << "input " << varCount << " ";
    for(auto& i : symbRow)
        ostr << i << " ";
    ostr << endl;
    ostr << "output " << funCount << " ";
    for(auto& i : result)
        ostr << i.first << " ";
    ostr << endl;
    for(auto& i : result)
        ostr << i.first << " = " << i.second << endl;

    ostr.close();
    return true;
}