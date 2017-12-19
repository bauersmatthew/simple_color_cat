#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

std::string const A_OFF     = "0";
std::string const A_BOLD    = "1";
std::string const A_UNDER   = "4";
std::string const A_BLINK   = "5";
std::string const A_REVERSE = "7";
std::string const A_CONCEAL = "8";
std::string const F_BLACK   = "30";
std::string const F_RED     = "31";
std::string const F_GREEN   = "32";
std::string const F_YELLOW  = "33";
std::string const F_BLUE    = "34";
std::string const F_MAGENTA = "35";
std::string const F_CYAN    = "36";
std::string const F_WHITE   = "37";
std::string const B_BLACK   = "40";
std::string const B_RED     = "41";
std::string const B_GREEN   = "42";
std::string const B_YELLOW  = "43";
std::string const B_BLUE    = "44";
std::string const B_MAGENTA = "45";
std::string const B_CYAN    = "46";
std::string const B_WHITE   = "47";

std::string translate(char c)
{
    switch(c)
    {
        case '0': return A_OFF+";"+F_WHITE+";"+B_BLACK;
        case 'o': return A_OFF;
        case 'd': return A_BOLD;
        case 'u': return A_UNDER;
        case '!': return A_BLINK;
        case '~': return A_REVERSE;
        case '?': return A_CONCEAL;
        case 'k': return F_BLACK;
        case 'r': return F_RED;
        case 'g': return F_GREEN;
        case 'y': return F_YELLOW;
        case 'b': return F_BLUE;
        case 'm': return F_MAGENTA;
        case 'c': return F_CYAN;
        case 'w': return F_WHITE;
        case 'K': return B_BLACK;
        case 'R': return B_RED;
        case 'G': return B_GREEN;
        case 'Y': return B_YELLOW;
        case 'B': return B_BLUE;
        case 'M': return B_MAGENTA;
        case 'C': return B_CYAN;
        case 'W': return B_WHITE;
    }
    return "";
}

void do_file(std::istream& fin)
{
    bool in_color_spec = false;
    bool first_attr = true;
    std::string new_color = "\e[";
    
    char c;
    fin.get(c);
    while(fin)
    {
        if(in_color_spec)
        {
            if(c == '\\')
            {
                // note: this makes it so that if a second \ is found anywhere
                // in the color sequence, the whole thing is translated into a
                // single \. For example: '\rB\]' --> '\'
                in_color_spec = false;
                first_attr = true;
                std::cout << '\\';
                new_color = "\e[";
            }
            else if(c == ']')
            {
                in_color_spec = false;
                first_attr = true;
                std::cout << new_color << 'm';
                new_color = "\e[";
            }
            else
            {
                if(!first_attr)
                {
                    new_color += ';';
                }
                else
                {
                    first_attr = false;
                }
                new_color += translate(c);
            }
        }
        else
        {
            if(c == '\\')
            {
                in_color_spec = true;
            }
            else
            {
                std::cout << c;
            }
        }
        fin.get(c);
    }
}

void print_help()
{
    std::cout
        << "Color code format: \\...]\n"
        << "Color codes:\n"
        << "\tAttributes\n"
        << "\t0\tdefault (white on black)\n"
        << "\to\toff\n"
        << "\td\tbold\n"
        << "\tu\tunderscore\n"
        << "\t!\tblink\n"
        << "\t~\treverse\n"
        << "\t?\tconceal\n\n"
        << "\tForegrounds\n"
        << "\tk\tblack\n"
        << "\tr\tred\n"
        << "\tg\tgreen\n"
        << "\ty\tyellow\n"
        << "\tb\tblue\n"
        << "\tm\tmagenta\n"
        << "\tc\tcyan\n"
        << "\tw\twhite\n\n"
        << "\tBackgrounds\n"
        << "\tK\tblack\n"
        << "\tR\tred\n"
        << "\tG\tgreen\n"
        << "\tY\tyellow\n"
        << "\tB\tblue\n"
        << "\tM\tmagenta\n"
        << "\tC\tcyan\n"
        << "\tW\twhite\n\n";
}

int main(int argc, char **argv)
{
    if(argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
    {
        print_help();
        return 0;
    }

    if(argc >= 2)
    {
        for(int i = 1; i < argc; i++)
        {
            std::ifstream fin(argv[i], std::ifstream::in|std::ifstream::binary);
            do_file(fin);
            fin.close();
        }
    }
    else
    {
        do_file(std::cin);
    }
    return 0;
}
