#include "Reader.h"

using namespace std;

Reader::Reader(const string& fname): fileName(fname)
{
    istr.open(fileName.c_str());
    if(!istr)
        good = false;
    else
    {
        good = true;
        errcount = 0;
    }
}

Reader::~Reader()
{
    istr.close();
}

void Reader::alert(int errcode, const TextPos &tp, const string &expl, const string& what)
{
    ++errcount;
    if(firstErrorInLine)
    {
        firstErrorInLine = false;
        std::cout << "Line " << pos.lineNumber << ": " << line;
    }

    cout << what << " #" << errcode << " at column " << tp.columnNumber << ": " << expl << endl;
}

char Reader::nextChar()
{
    ++pos.columnNumber;
    int c;
    if((c = istr.get()) == '\n')
    {
        ++pos.lineNumber;
        pos.columnNumber = 0;
        firstErrorInLine = true;
    }
    if(c == EOF)
        return EOF;
    return (char)c;
}

