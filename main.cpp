//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include <unordered_map>
#include "FuzzyFunction.h"
#include "ImplicTable.h"

using namespace std;


int main()
{
    //FuzzyFunction f(list<Implic>{Implic(list<SymbInstance>{SymbInstance(0, true), SymbInstance(0, false)}), Implic(list<SymbInstance>{SymbInstance(1, true), SymbInstance(0, true)})});
    /*SymbInstance a(1, true), b(1, false), c(1, false), d(0, true);
    list<SymbInstance> dave;
    dave.push_back(a);
    dave.push_back(b);
    list<SymbInstance> connor;
    connor.push_back(c);
    connor.push_back(d);
    Implic alpha(dave), beta(connor);
    list<Implic> steve;
    steve.push_back(alpha);
    steve.push_back(beta);
    FuzzyFunction f(steve);
    cout << f.calc(vector<double>{0.5, 0.25}) << endl;*/

    vector<int> v1, v2, v3, v4, v5, v6, v7;
    list<vector<int>> l;
    v1.push_back(0);
    v1.push_back(1);
    v1.push_back(0);
    v1.push_back(1);
    v2.push_back(2);
    v2.push_back(2);
    v2.push_back(1);
    v2.push_back(0);
    v3.push_back(1);
    v3.push_back(2);
    v3.push_back(0);
    v3.push_back(2);
    v4.push_back(2);
    v4.push_back(1);
    v4.push_back(2);
    v4.push_back(2);
    v5.push_back(1);
    v5.push_back(2);
    v5.push_back(3);
    v5.push_back(1);
    v6.push_back(3);
    v6.push_back(1);
    v6.push_back(1);
    v6.push_back(2);
    v7.push_back(2);
    v7.push_back(2);
    v7.push_back(2);
    v7.push_back(3);
    l.push_back(v1);
    l.push_back(v2);
    l.push_back(v3);
    l.push_back(v4);
    l.push_back(v5);
    l.push_back(v6);
    l.push_back(v7);

    ImplicTable i(l);
//    i.minimizeExact();
    i.minimizeHeuristic();
    i.print();

/*    vector<double> v{0.5, 0.75};
    cout << v[0] << endl;
    cout << v[1] << endl;
    cout << *++(list<double>{0.5, 0.25}.begin()) << endl;*/
}