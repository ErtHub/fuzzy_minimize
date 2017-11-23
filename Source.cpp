// Modu SOURCE.CPP
// =================
// Funkcje skadowe klasy Source.
//
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
    cout << endl << "Interpreter Analang, v0.1, zadne prawa niezastrzeone" << endl;
    cout << "Raport wykonania dla pliku: \""<<fnam<<"\"" << endl << endl;
    etotal = 0;
    NextLine();   // Pierwszy wiersz
}

Source::~Source()
{
//LastMessage
    cout << endl << "fuzzy_minimize: koniec raportu. Wykrytych bw: " << etotal << endl;
    istr.close();
}

bool Source::NextLine()	// Zwraca true jeli jest nastny wiersz
{
    if(istr.eof())
        return false;
    getline(istr, Line);  // Pobiera wiersz (bez znaku '\n')
    Line.push_back('\n');

    ++tpos.ln;	// Nastpny wiersz
    tpos.cn=0;	// Przed pierwszym znakiem
    while(Line[tpos.cn] == ' ' || Line[tpos.cn] == '\t')
        ++tpos.cn;
    PrntLine();

    einline=0;	// 0 bw w tym wierszu
    return true;
}

void Source::Error(int ec, const TextPos& tp, const string& mt, const string& at)
{
    ++etotal;
    if(einline == 0)	// Jest pierwszy bd w wierszu
        cout << setw(5) << tpos.ln << ' ' << Line;	// Druk wiersza owego

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

