#include "Source.h"

using namespace std;

Source::Source(const string& fname): fileName(fname)
{
    istr.open(fileName.c_str());
    if(!istr)
        good = false;
    else
    {
        good = true;
        cout << "Report of procedure for file: \""<<fname<<"\"" << endl << endl;
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

    ++pos.lineNumber;
    pos.charNumber=0;
    while(line[pos.charNumber] == ' ' || line[pos.charNumber] == '\t')
        ++pos.charNumber;

    einline=0;
    return true;
}

void Source::error(int errcode, const TextPos &tp, const string &expl)
{
    ++etotal;
    if(einline == 0)
    {
        printLine();
        einline=1;
    }
    else
        ++einline;


    cout << setw(11) << "Error #" << errcode;
    cout << setw(tp.charNumber) << setfill('-') << '^' << setfill(' ') << expl << endl;
}

char Source::nextChar()
{
    bool r = true;
    if(pos.charNumber == line.size())
        r = nextLine();
    if(r)
        return line[pos.charNumber++];
    else
        return EOF;
}

