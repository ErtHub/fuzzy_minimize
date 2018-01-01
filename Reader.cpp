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
        nextLine();
    }
}

Reader::~Reader()
{
    istr.close();
}

bool Reader::nextLine()
{
    if(istr.eof())
        return false;
    getline(istr, line);
    line.push_back('\n');

    ++pos.lineNumber;
    pos.columnNumber = 0;
    while(line[pos.columnNumber] == ' ' || line[pos.columnNumber] == '\t')
        ++pos.columnNumber;

    firstErrorInLine = true;
    return true;
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
    bool r = true;
    if(pos.columnNumber == line.size())
        r = nextLine();
    if(r)
        return line[pos.columnNumber++];
    else
        return EOF;
}

