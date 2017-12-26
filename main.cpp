//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include <memory>
#include "CubeTable.h"
#include "Scanner.h"
#include "Parser.h"
#include "FunctionGenerator.h"

using namespace std;


int Trace::callLevel = 0;//TODO pozbyc sie...?
int Trace::traceOn = 0;
int Trace::showSymbols = 0;

int main(int argc, char* argv[])
{

    int options = 0;

    string filename;

    shared_ptr<Minimizer> minimizer;

    enum MinimizationFashion
    {
        EXACT = 0, HEURISTIC, HEURISTIC_MUKAIDONO
    };

    enum Writers
    {
        NO_WRITER = 0, VERBOSE = 4, VERY_VERBOSE = 8
    };

    enum Options
    {
        ALGORITHM = 3, WRITER = 12, TIMER = 16, GENERATE = 32
    };//TODO wyprowadzic te enumy do respektywnych modulow


    if (argc < 2)
    {
        cout << "Insufficient execution arguments!" << endl;
        return -1;
    }

    unsigned long genV = 1, genC = 1, genF = 1;

    for(int i = 1; i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'e':
                    options |= EXACT;
                    break;
                case 'h':
                    options |= HEURISTIC;
                    break;
                case 'm':
                    options |= HEURISTIC_MUKAIDONO;
                    break;
                case 'v':
                    options |= VERBOSE;
                    break;
                case 'V':
                    options |= VERY_VERBOSE;
                    break;
                case 't':
                    options |= TIMER;
                    break;
                case 'g':
                    options |= GENERATE;
                    if(++i >= argc || (genV = strtoul(argv[i], nullptr, 0)) == 0)
                    {
                        genF = genC = genV = 1;
                        --i;
                    }
                    else if(++i >= argc || (genC = strtoul(argv[i], nullptr, 0)) == 0)
                    {
                        genC = genV;
                        genF = 1;
                        --i;
                    }
                    else if(++i >= argc || (genF = strtoul(argv[i], nullptr, 0)) == 0)
                    {
                        genF = 1;
                        --i;
                    }
                    break;
                default:
                    cout << "Unknown option \"" << argv[i] << "\"!" << endl;
                    return -3;
            }
        }
        else if(filename.empty())
            filename = argv[i];
        else
        {
            cout << "Unknown option \"" << argv[i] << "\"!" << endl;
            return -3;
        }
    }

    if(filename.empty())
    {
        cout << "File name not defined." << endl;
        return -4;
    }

    if(options & GENERATE)
    {
        cout << genV << " " << genC << " " << genF << endl;
        FunctionGenerator fg(genV, genC, genF);
        fg.generate();
        if(!fg.writeToFile(filename))
        {
            cout << "Could not write to file \"" << filename << "\"" << endl;
            return -6;
        }
        return 0;
    }

    switch(options & ALGORITHM)
    {
        case EXACT:
            minimizer = shared_ptr<Minimizer>(new ExactMinimizer((options & WRITER) / VERBOSE));
            break;
        case HEURISTIC:
            minimizer = shared_ptr<Minimizer>(new HeuristicMinimizer((options & WRITER) / VERBOSE));
            break;
        case HEURISTIC_MUKAIDONO:
            minimizer = shared_ptr<Minimizer>(new MukaidonoMinimizer((options & WRITER) / VERBOSE));
            break;
        default:
            cout << "Algorithm option error." << endl;
            return -5;
    }

    if(options & TIMER)
        minimizer = shared_ptr<Minimizer>(new Timer(minimizer));

    Source src(filename);
    if(!src.isGood())
    {
        cout << "Could not read file \"" << filename << "\"" << endl;
        return -7;
    }
    Scanner scn(src);
    Parser par(scn);
    Sync::p = &par;
    if(par.parseProgram())
    {
        list<pair<string, FuzzyFunction>> funsToMinimize = move(par.extract());
        for(auto& i : funsToMinimize)
            i.second = move(i.second.minimize(minimizer.get()));
        for(auto& i : funsToMinimize)
            cout << i.first << " = " << i.second << endl;
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

    /*vector<uint8_t> v1, v2, v3, v4, v5, v6, v7;
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


    /*vector<uint8_t> v1, v2, v3, v4, v5, v6, v7;
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