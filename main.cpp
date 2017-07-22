//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include <unordered_map>
#include "FuzzyFunction.h"

using namespace std;


int main()
{
    //FuzzyFunction f(list<Kos>{Kos(list<SymbInstance>{SymbInstance(0, true), SymbInstance(0, false)}), Kos(list<SymbInstance>{SymbInstance(1, true), SymbInstance(0, true)})});
    SymbInstance a(1, true), b(1, false), c(1, false), d(0, true);
    list<SymbInstance> dave;
    dave.push_back(a);
    dave.push_back(b);
    list<SymbInstance> connor;
    connor.push_back(c);
    connor.push_back(d);
    Kos alpha(dave), beta(connor);
    list<Kos> steve;
    steve.push_back(alpha);
    steve.push_back(beta);
    FuzzyFunction f(steve);
    cout << f.calc(vector<double>{0.5, 0.25}) << endl;
/*    vector<double> v{0.5, 0.75};
    cout << v[0] << endl;
    cout << v[1] << endl;
    cout << *++(list<double>{0.5, 0.25}.begin()) << endl;*/
}