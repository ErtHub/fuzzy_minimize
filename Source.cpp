#include "Source.h"

using namespace std;

Source::Source(const string& fnam): fn(fnam)
{
    istr.open(fn.c_str());
    if(!istr)
        good = false;
    else
    {
        good = true;
        cout << "Report of procedure for file: \""<<fnam<<"\"" << endl << endl;
        etotal = 0;
        nextLine();
    }
}

Source::~Source()
{
    cout << endl << "End of execution. Read errors detected: " << etotal << endl;
    istr.close();
}

bool Source::nextLine()
{
    if(istr.eof())
        return false;
    getline(istr, line);
    line.push_back('\n');

    ++tpos.ln;
    tpos.cn=0;
    while(line[tpos.cn] == ' ' || line[tpos.cn] == '\t')
        ++tpos.cn;
    prntLine();

    einline=0;
    return true;
}

void Source::error(int ec, const TextPos &tp, const string &mt, const string &at)
{
    ++etotal;
    if(einline == 0)
        cout << setw(5) << tpos.ln << ' ' << line;

    einline=1;

    cout << setw(2) << ec << "*** ";
    cout << setw(tp.cn) << setfill('-') << '^' << setfill(' ') << mt << at << endl;
}

char Source::nextChar()
{
    bool r = true;
    if(tpos.cn == line.size())
        r = nextLine();
    if(r)
        return line[tpos.cn++];
    else
        return EOF;
}

