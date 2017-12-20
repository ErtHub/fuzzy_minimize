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
    uniform_int_distribution<uint8_t> uni(0, 3);

    for(unsigned long i = 0; i < cCount; ++i)
    {
        CubeRow row(vCount);
        bool cubeShallNotBeEmpty = false;

        for(unsigned long v = 0; v < vCount; ++v)
            row.set(uni(rng), v);
        //we don't want any all-0 cubes to be created, so we choose a random position to put a random [1; 3] value there:
        uniform_int_distribution<unsigned long> tweakerWhere(0, vCount - 1);
        uniform_int_distribution<uint8_t> tweakerWhat(1, 3);
        row.set(tweakerWhat(rng), tweakerWhere(rng));

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
    cout << "1" << endl;
    ofstream ostr;

    ostr.open(fn.c_str());
    if(!ostr)
        return false;

    ostr << "input " << varCount << " ";
    for(auto& i : varTable)
        ostr << i.first << " ";
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