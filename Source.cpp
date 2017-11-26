#include "Source.h"

using namespace std;

Source::Source(const string& fnam): fn(fnam)
{
    istr.open(fn.c_str());
    if(!istr)
    {
        cout<<"fuzzy_minimize: error reading file \"" << fn << "\"" << endl;
        exit(1);
    }
    cout << "Report of procedure for file: \""<<fnam<<"\"" << endl << endl;
    etotal = 0;
    NextLine();
}

Source::~Source()
{
    cout << endl << "fuzzy_minimize: End of report. Errors detected: " << etotal << endl;
    istr.close();
}

bool Source::NextLine()
{
    if(istr.eof())
        return false;
    getline(istr, Line);
    Line.push_back('\n');

    ++tpos.ln;
    tpos.cn=0;
    while(Line[tpos.cn] == ' ' || Line[tpos.cn] == '\t')
        ++tpos.cn;
    PrntLine();

    einline=0;
    return true;
}

void Source::Error(int ec, const TextPos& tp, const string& mt, const string& at)
{
    ++etotal;
    if(einline == 0)
        cout << setw(5) << tpos.ln << ' ' << Line;

    einline=1;

    cout << setw(2) << ec << "*** ";
    cout << setw(tp.cn) << setfill('-') << '^' << setfill(' ') << mt << at << endl;
}

char Source::NextChar()
{
    bool r = true;
    if(tpos.cn == Line.size())
        r = NextLine();
    if(r)
        return Line[tpos.cn++];
    else
        return EOF;
}

