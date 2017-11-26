//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include "ImplicTable.h"
#include "Scan.h"
#include "Source.h"
#include "Parser.h"
#include "MinimizeAlgorithms.h"

using namespace std;


int Trace::call_level = 0;
int Trace::trace_on = 0;
int Trace::show_symbols = 0;

int main(int argc, char* argv[])
{

    if (argc < 3)
    {
        cout << "Insufficient execution arguments!" << endl;
        return -1;
    }

    int howToMinimize = 0;

    if(argv[1][0] == '-')
    {
        switch(argv[1][1])
        {
            case 'e':
                howToMinimize = 0;
                break;
            case 'h':
                howToMinimize = 1;
                break;
            case 'm':
                howToMinimize = 2;
                break;
            default:
                cout << "Unknkown option \"" << argv[1] << "\"!" << endl;
                return -3;
        }
    }
    else
    {
        cout << "Unknown option \"" << argv[1] << "\"!" << endl;
        return -3;
    }


    string filename(argv[2]);
//    std::cout << "File name:" << endl;
//    cin >> filename;
    Source src(filename);
    Scan scn(src);
    Parser par(scn);
    Synchronize::p = &par;
    if(par.Program())
    {
        /*FuzzyFunction f(par.extract());
        f.minimize();*/
        cout << FuzzyFunction(par.extract()).minimize(howToMinimize) << endl;
        return 0;
    }
    else
    {
        cout << "Syntax error reading file" << endl;
        return -2;
    }
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

    /*vector<unsigned char> v1, v2, v3, v4, v5, v6, v7;
    list<ImplicRow> l;
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
    l.emplace_back(ImplicRow(v1));
    l.emplace_back(ImplicRow(v2));
    l.emplace_back(ImplicRow(v3));
    l.emplace_back(ImplicRow(v4));
    l.emplace_back(ImplicRow(v5));
    l.emplace_back(ImplicRow(v6));
    l.emplace_back(ImplicRow(v7));*/


    /*vector<unsigned char> v1, v2, v3, v4, v5, v6, v7;
    list<ImplicRow> l;
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
    l.emplace_back(ImplicRow(v1));
    l.emplace_back(ImplicRow(v2));
    l.emplace_back(ImplicRow(v3));
    l.emplace_back(ImplicRow(v4));
    l.emplace_back(ImplicRow(v5));
    l.emplace_back(ImplicRow(v6));
    l.emplace_back(ImplicRow(v7));

    ImplicTable i(l);
//    i.minimizeExact();
//    i.minimizeHeuristic();
    i.minimizeMukaidono();
    i.print();*/

/*    vector<double> v{0.5, 0.75};
    cout << v[0] << endl;
    cout << v[1] << endl;
    cout << *++(list<double>{0.5, 0.25}.begin()) << endl;*/
}