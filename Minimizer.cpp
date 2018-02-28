//
// Created by hubert on 08.12.17.
//

#include "Minimizer.h"

using namespace std;
using namespace std::chrono;

FuzzyFunction ExactMinimizer::minimize(const FuzzyFunction &input)
{
    clear();
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

#ifdef SORT_TABLES
    tab.sort();
#endif
    tab.sweepCovered();

    givenFunction = tab;

    tab.minimizeExact();
    if(expand)
    {
        tab.chooseCoveringSubset();
        result = FuzzyFunction(input.getVarTable(), tab);
        return result;
    }
    result = FuzzyFunction(input.getVarTable(), tab);
    essentials = move(tab.separateEssentials());
    redundants = vector<CubeRow>(tab.size());
    for(auto& i : redundants)
        i = move(tab.pop_front());
    CubeTableCont uncoveredCompletes = givenFunction.findUncoveredCompletes(essentials);

    if(!uncoveredCompletes.empty())
    {
        for(unsigned long i = 0; i < redundants.size(); ++i)
        {
            if(!redundants[i].covers(uncoveredCompletes.front()))
                continue;
            CubeRow row(redundants.size());
            row.set(1, i);
            solutions.append(row);
        }
        uncoveredCompletes.pop_front();
        
        while(!uncoveredCompletes.empty())
        {
            CubeTable factor2;
            for(unsigned long i = 0; i < redundants.size(); ++i)
            {
                if(!redundants[i].covers(uncoveredCompletes.front()))
                    continue;
                CubeRow row(redundants.size());
//            cout << redundants.size() << "hubert" << endl;
                row.set(1, i);
//            cout << row << "hubert" << endl;
                factor2.append(row);
            }
//        cout << solutions << "hubert" << factor2 << "hubert" << endl;
            solutions = move(solutions.crossProduct(factor2));
            uncoveredCompletes.pop_front();
        }
    }
    else
    {
        CubeRow row(redundants.size());
        solutions.append(row);
    }

    while(!solutions.empty())
    {
//        cout << "hubert" << endl << solutions << "hubert" << endl;
        CubeTable covering(essentials);
        CubeRow sol = move(solutions.pop_front());
        for(unsigned long i = 0; i < sol.size(); ++i)
            if(sol.get(i))
                covering.append(redundants[i]);
        coverings.emplace_back(FuzzyFunction(input.getVarTable(), covering));
    }

    return result;
}

FuzzyFunction HeuristicMinimizer::minimize(const FuzzyFunction &input)
{
    clear();
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

    tab.minimizeHeuristic();

    if(expand)
        tab.chooseCoveringSubset();

    if(write)
        cout << tab << endl << "================" << endl;

    result = FuzzyFunction(input.getVarTable(), tab);
    return result;
}

FuzzyFunction MukaidonoMinimizer::minimize(const FuzzyFunction &input)
{
    clear();
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

    tab.minimizeMukaidono();

    if(expand)
        tab.chooseCoveringSubset();

    if(write)
        cout << tab << endl << "================" << endl;

    result = FuzzyFunction(input.getVarTable(), tab);
    return result;
}

FuzzyFunction Timer::minimize(const FuzzyFunction &input)
{
    high_resolution_clock::time_point executionStart = high_resolution_clock::now();
    result = wrappee->minimize(input);
    high_resolution_clock::time_point executionEnd = high_resolution_clock::now();

    duration<double, ratio<1, 1000>> executionTime = executionEnd - executionStart;
    timeRecords.push_back(executionTime.count());
    return result;
}

void Minimizer::clear()
{
    result.clear();
}

void ExactMinimizer::clear()
{
    Minimizer::clear();
    solutions.clear();
    essentials.clear();
    givenFunction.clear();
    redundants.clear();
    coverings.clear();
}

ostream& Minimizer::writeResult(std::ostream &os) const
{
    os << result << endl;
    return os;
}

ostream& ExactMinimizer::writeResult(std::ostream &os) const
{
    unsigned long enumerator = 0;
    Minimizer::writeResult(os);
    if(!expand)
    {
        os << "Possible coverings:" << endl;
        for(auto& i : coverings)
            os << ++enumerator << ". " << i << endl;
    }
    return os;
}

ostream& Timer::writeResult(std::ostream &os) const
{
    return wrappee->writeResult(os);
}

ostream& Timer::report(std::ostream &os) const
{
    unsigned long enumerator = 0;

    wrappee->report(os) << "Execution times:" << endl;
    for(auto t : timeRecords)
        os << ++enumerator << ". " << t << " ms" << endl;
    return os;
}
