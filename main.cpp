//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include <memory>
#include "CubeTable.h"
#include "Scan.h"
#include "Parser.h"

using namespace std;


int Trace::call_level = 0;
int Trace::trace_on = 0;
int Trace::show_symbols = 0;

int main(int argc, char* argv[])
{

    shared_ptr<Minimizer> minimizer;

    if (argc < 3)
    {
        cout << "Insufficient execution arguments!" << endl;
        return -1;
    }

    if(argv[1][0] == '-')
    {
        switch(argv[1][1])
        {
            case 'e':
                minimizer = shared_ptr<Minimizer>(new ExactMinimizer());
                break;
            case 'h':
                minimizer = shared_ptr<Minimizer>(new HeuristicMinimizer());
                break;
            case 'm':
                minimizer = shared_ptr<Minimizer>(new MukaidonoMinimizer());
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
//        cout << FuzzyFunction(par.extract());
        cout << FuzzyFunction(par.extract()).minimize(minimizer.get()) << endl;
        return 0;
    }
    else
    {
        cout << "Syntax error reading file" << endl;
        return -2;
    }
    //FuzzyFunction f(list<Cube>{Cube(list<SymbInstance>{SymbInstance(0, true), SymbInstance(0, false)}), Cube(list<SymbInstance>{SymbInstance(1, true), SymbInstance(0, true)})});
    /*SymbInstance a(1, true), b(1, false), c(1, false), d(0, true);
    list<SymbInstance> dave;
    dave.push_back(a);
    dave.push_back(b);
    list<SymbInstance> connor;
    connor.push_back(c);
    connor.push_back(d);
    Cube alpha(dave), beta(connor);
    list<Cube> steve;
    steve.push_back(alpha);
    steve.push_back(beta);
    FuzzyFunction f(steve);
    cout << f.calc(vector<double>{0.5, 0.25}) << endl;*/

    /*vector<unsigned char> v1, v2, v3, v4, v5, v6, v7;
    list<CubeRow> l;
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
    l.emplace_back(CubeRow(v1));
    l.emplace_back(CubeRow(v2));
    l.emplace_back(CubeRow(v3));
    l.emplace_back(CubeRow(v4));
    l.emplace_back(CubeRow(v5));
    l.emplace_back(CubeRow(v6));
    l.emplace_back(CubeRow(v7));*/


    /*vector<unsigned char> v1, v2, v3, v4, v5, v6, v7;
    list<CubeRow> l;
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
    l.emplace_back(CubeRow(v1));
    l.emplace_back(CubeRow(v2));
    l.emplace_back(CubeRow(v3));
    l.emplace_back(CubeRow(v4));
    l.emplace_back(CubeRow(v5));
    l.emplace_back(CubeRow(v6));
    l.emplace_back(CubeRow(v7));

    CubeTable i(l);
//    i.minimizeExact();
//    i.minimizeHeuristic();
    i.minimizeMukaidono();
    i.print();*/

/*    vector<double> v{0.5, 0.75};
    cout << v[0] << endl;
    cout << v[1] << endl;
    cout << *++(list<double>{0.5, 0.25}.begin()) << endl;*/
}