#ifndef _UTILS_H
#define _UTILS_H

#include <curses.h>
#include <string>

using std::string;

namespace sqlpp {


void clearScreen(WINDOW* scr)
{
    // clean all terminal 
    int r, c;
    getyx(scr, r, c);
    move(r, 0);
    
    while(r != 0)
        deleteln(); move(--r, 0);
    
    deleteln();
    refresh();
}

void deleteToPosition(WINDOW* scr, int colnr)
{
    int c, r;
    getyx(scr, r, c);
    while(c > colnr)
        mvdelch(r, --c);
    
    refresh();
}

void insertChar(WINDOW* scr, const string& prompt, string& str, char ch)
{
     int row, col;
     getyx(scr, row, col);
     str.insert(col - (int)prompt.size(), 1, ch);
     
     deleteToPosition(scr, 0);
     string line = prompt + str;
     mvprintw(row, 0, line.c_str()); 
     move(row, ++col);
     
     refresh();
}

void deleteChar(WINDOW* scr, const string& prompt, string& str)
{
    int row, col;
    getyx(scr, row, col); 
    if (col > (int)prompt.size())
    {
        // find position before the cursor.
        int pos = col - (int)prompt.size() - 1;
        // only erase char at tha position.
        str.erase(str.begin() + pos);
       
        // because we have modified the string, we replace that strig with the new string at the current row(line)
        deleteToPosition(scr, 0);
        string line = prompt + str;
        mvprintw(row, 0, line.c_str()); 
        move(row, --col);
    }

    refresh(); 
}



} // namespace sqlpp


#endif
