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
   char info[24] = "sqlpp>"; 
   string msg, out;
   int row, col;
   int tabhit;
   size_t found; 

   string word;
   vector<string> completions;
   keywowds_.insert("select");
   keywowds_.insert("selector");
   keywowds_.insert("selectivity");
   keywowds_.insert("seeyou");

   mvprintw(0,0, info);
   refresh();

   while(1)
   {
        ch = getch();
        switch(ch)
        {
            case 0x08:
                delch();
                addstr("back key was pressed!");
            // tab key == 0x09
            case 0x09:
                // do some completion.
                addstr("tab tab");
                found = msg.rfind(" ");
                if (found != string::npos)
                    word = msg.substr(found); 
                else
                    word = msg;
     
                searchWord(word, completions);
                if (!completions.empty() && tabhit < (int)completions.size())
                    mvaddstr(row, found, completions[tabhit].c_str());
                if (!completions.empty() && tabhit >= (int)completions.size())
                {
                    tabhit = 0;
                    mvaddstr(row, found, completions[tabhit].c_str());
                }
                ++tabhit;
                refresh();

             // enter key 
            case 0x0a:
                // do something with string processing.
                ++row;
                out = sql_.parse(msg);
                mvprintw(row, 0, out.c_str());
                refresh();
            default:
                addch(ch);
                msg.append((const char*)&ch);
                refresh();
                
        }
    } 
} // end of operator()

void Console::searchWord(const string& str, vector<string>& result)
{
    result.clear();

    if (str.empty())
        return; 

    std::set<string>::const_iterator cit;
    for (cit = keywowds_.begin(); cit != keywowds_.end(); ++cit)
    {
       string temp = *cit;
       int res = temp.compare(0, str.size(), str); 
       if (res == 0)
           result.push_back(temp);
    }

}

