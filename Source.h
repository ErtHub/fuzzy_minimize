// Modu SOURCE.H
// ===============
// Definicja klasy Source.
//
#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>


//extern int options;	// Definicja w funkcji main()

struct TextPos	// Pomocnicza klasa: pozycja w pliku tekstowym
{
    int ln;     // Numer wiersza (od 1)
    int cn;     // Numer znaku w wierszu (od 1)
    explicit TextPos(int l = 0, int c = 0): ln(l), cn(c) {}
};

class Source
{
    int etotal, einline;// Liczniki bdw: globalny, w wierszu

    const std::string fn;    // Nazwa pliku
    std::ifstream istr;
    std::string Line;        // Bufor biecego wiersza

    TextPos tpos;     	// Bieca pozycja w pliku

    void PrntLine()		  // Drukowanie biecego wiersza
    {
        std::cout << std::setw(5) << tpos.ln << ' ' << Line;
    }

    bool NextLine();		// pny wiersz do bufora

public:
    explicit Source(const std::string& file);
    ~Source();
    void Error(int ec, const TextPos& tp, const std::string& mt = "", const std::string& at = "");
    char NextChar();
    const TextPos& GetPos() const
    {
        return tpos;
    }
};
#endif
