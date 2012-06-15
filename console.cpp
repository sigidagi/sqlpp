#include "console.h"
#include <curses.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using std::string;

Console::Console()
{
    initscr();
    clear();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
}

Console::~Console()
{
    endwin();
}

void Console::operator()() 
{
   int ch;   
   string info = "sqlpp>"; 
   string msg, out;
   int row(0), col(0);
   int tabhit(0);
   size_t found(0); 

   string word;
   vector<string> completions;
   keywowds_.insert("select");
   keywowds_.insert("selector");
   keywowds_.insert("selectivity");
   keywowds_.insert("seeyou");

   mvprintw(0,0, info.c_str());
   refresh();

   while(1)
   {
        ch = getch();
        //if (ch == 0x08)
        if (ch == KEY_BACKSPACE)
        {
            getyx(stdscr, row, col);
            if (col > (int)info.size())
                mvdelch(row, col-1);
        }
        if (ch == ' ')
        {
            // check if completion was called.
            if (!completions.empty())
                msg = msg.substr(0, found+1) + completions[tabhit-1];

            addch(ch);
            msg.append((const char*)&ch);
        }
        // tab key == 0x09
        else if (ch == 0x09)
        {
            // delete char '\t'
            delch();
            // do some completion.
            found = msg.rfind(" ");
            if (found != string::npos)
                word = msg.substr(found+1); 
            else
                word = msg;
            searchWord(word, completions);
            if (!completions.empty() && tabhit < (int)completions.size())
            {
                deleteToPosition(info.size()+found+1);
                mvprintw(row, info.size()+found+1, completions[tabhit].c_str());
            }
            if (!completions.empty() && tabhit >= (int)completions.size())
            {
                tabhit = 0;
                deleteToPosition(info.size()+found+1);
                mvprintw(row, info.size()+found+1, completions[tabhit].c_str());
            }
            ++tabhit;
         }
         // enter key 
         else if (ch == 0x0a)
         {
                // do something with string processing.
             ++row;
             out = sql_.parse(msg);
             // print whatever it cames out: error  or result.
             if (!out.empty())
             {
                mvprintw(row, 0, out.c_str());
                ++row;
             }
             mvprintw(row, 0, info.c_str());
         }
         else
         {
             addch(ch);
             msg.append((const char*)&ch);
         }

         refresh();
    } // end of while  
} // end of operator()

void Console::deleteToPosition(int colnr)
{
    int c, r;
    getyx(stdscr, r, c);
    while(c > colnr)
    {
        mvdelch(r, --c);
    }
}

void Console::searchWord(const string& str, vector<string>& result)
{
    result.clear();

    if (str.empty())
        return; 

    std::set<string>::const_iterator cit;
    for (cit = keywowds_.begin(); cit != keywowds_.end(); ++cit)
    {
       string tmp = *cit;
       int res = tmp.compare(0, str.size(), str); 
       if (res == 0)
           result.push_back(tmp);
    }

}

