//
// Created by hubert on 08.12.17.
//

#include "Minimizer.h"

using namespace std;

FuzzyFunction ExactMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    tab.minimizeExact();

    if(write)
        cout << tab << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}

FuzzyFunction HeuristicMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    tab.minimizeHeuristic();

    if(write)
        cout << tab << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}

FuzzyFunction MukaidonoMinimizer::minimize(const FuzzyFunction &input)
{
    CubeTable tab(input, write);

    tab.minimizeMukaidono();

    if(write)
        cout << tab << endl;

    return FuzzyFunction(input.getVarTable(), tab);
}