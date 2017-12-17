//
// Created by hubert on 08.12.17.
//

#include "Minimizer.h"

using namespace std;
using namespace std::chrono;

FuzzyFunction ExactMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

    tab.minimizeExact();

    if(write)
        cout << tab << endl << "================" << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}

FuzzyFunction HeuristicMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

    tab.minimizeHeuristic();

    if(write)
        cout << tab << endl << "================" << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}

FuzzyFunction MukaidonoMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    if(write)
        cout << "================" << endl;

    tab.minimizeMukaidono();

    if(write)
        cout << tab << endl << "================" << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}

FuzzyFunction Timer::minimize(const FuzzyFunction &input)
{
    FuzzyFunction result;

    high_resolution_clock::time_point executionStart = high_resolution_clock::now();
    result = move(wrappee->minimize(input));
    high_resolution_clock::time_point executionEnd = high_resolution_clock::now();

    duration<double, ratio<1, 1000>> executionTime = executionEnd - executionStart;
    timeRecords.push_back(executionTime.count());
    return result;
}

Timer::~Timer()
{
    unsigned long enumerator = 0;

    cout << "Execution times:" << endl;
    for(auto& t : timeRecords)
        cout << ++enumerator << ". " << t << " ms" << endl;
}