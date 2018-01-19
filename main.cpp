//
// Created by hubert on 11.07.17.
//

#include <iostream>
#include <memory>
#include <getopt.h>
#include <sstream>
#include "CubeTable.h"
#include "Scanner.h"
#include "Parser.h"
#include "FunctionGenerator.h"
#include "Minimizer.h"

using namespace std;

void printHelp(char* progname, ostream& os)
{
    os << progname << " - Help" << endl;
    os << progname <<" [-f filename][-e|-u|-m][-v|-V][-t][-r genV][-c genC][-i genF][-h]" << endl;
    os << "Options:" << endl;
    os << "-f - specify the name of the file to be read from or written to" << endl;
    os << "-e, -u, -m - specify the algorithm to be run, Exact, Heuristic and Mukaidono respectively" << endl;
    os << "-v - print cube tables every iteration" << endl;
    os << "-V - print cube tables and fuzzy consensus calculation every iteration" << endl;
    os << "-t - measure the algorithms execution time" << endl;
    os << "-r, -c, -i - launch program in the random fuzzy function generation mode instead and specify the number of variables, cubes and functions respectively" << endl;
    os << "-h - print this help" << endl;
}

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

    if(argc < 2)
    {
        cout << "Usage:" << endl;
        cout << argv[0] << " -h - ask for help" << endl;
        cout << argv[0] << " -f filename - run the Exact Algorithm for the content of the file with name \"filename\"" << endl;
        return -1;
    }

    unsigned long genV = 0, genC = 0, genF = 0;
    int c;

    while((c = getopt(argc, argv, "f:eumvVtr:c:i:h")) != -1)
    {
        switch(c)
        {
            case 'f':
                filename = optarg;
                break;
            case 'e':
                options |= EXACT;
                break;
            case 'u':
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
            case 'r':
                options |= GENERATE;
                genV = strtoul(optarg, nullptr, 0);
                break;
            case 'c':
                options |= GENERATE;
                genC = strtoul(optarg, nullptr, 0);
                break;
            case 'i':
                options |= GENERATE;
                genF = strtoul(optarg, nullptr, 0);
                break;
            case 'h':
                printHelp(argv[0], cout);
                return 0;
            case '?':
            default:
                if(optopt == 'f' || optopt == 'r' || optopt == 'c' || optopt == 'i')
                    cout << "Option \"-" << (char)optopt << "\" requires an argument!" << endl;
                else
                    cout << "Unknown option \"-" << (char)optopt << "\"!" << endl;
                return -2;
        }
    }

    if(filename.empty())
    {
        cout << "File name not defined." << endl;
        return -3;
    }

    if(options & GENERATE)
    {
        if(!genV)
            genV = 1;
        if(!genC)
            genC = genV;
        if(!genF)
            genF = 1;
        cout << "Generating " << genF << genV << " variable fuzzy functions, " << genC << " cubes each...";
        FunctionGenerator fg(genV, genC, genF);
        fg.generate();
        if(!fg.writeToFile(filename))
        {
            cout << "Could not write to file \"" << filename << "\"" << endl;
            return -4;
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

    Reader rdr(filename);
    if(!rdr.isGood())
    {
        cout << "Could not read file \"" << filename << "\"" << endl;
        return -6;
    }
    else
        cout << "Reading file \"" << filename << "\"..." << endl;
    Scanner scn(rdr);
    Parser par(scn);
    if(par.parseProgram())
    {
        cout << "Done reading file. Errors detected: " << rdr.getErrcount() << endl;
        list<pair<string, FuzzyFunction>> funsToMinimize = move(par.extract());
        ostringstream ostr;
        for(auto& i : funsToMinimize)
        {
            minimizer->minimize(i.second);
            minimizer->writeResult(ostr << i.first << " = ");
            ostr << endl;
        }
        cout << ostr.str();
        minimizer->report(cout);
        return 0;
    }
    else
    {
        cout << "Critical syntax error reading file" << endl;
        return -7;
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