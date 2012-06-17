#include "console.h"
#include "orasql.h"
#include <curses.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using std::string;

Console::Console() : info_("sqlpp>")
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    sql_ = new OraSql(this);
}

Console::~Console()
{
    endwin();
}

int Console::exec() 
{
   int ch;   
   string msg;
   int row(0), col(0);
   int tabhit(0), uphit(0);
   size_t found(0); 

   string word;
   // only those keywords which fits typed word.
   vector<string> completions;

   clearScreen();

   while(1)
   {
        ch = getch();
        //if (ch == 0x08)
        if (ch == KEY_BACKSPACE || ch == 0x08)
        {
            delch();
            getyx(stdscr, row, col);
            if (col > (int)info_.size())
            {
                mvdelch(row, col-1);
                msg.erase(msg.end()-1);
            }
        }
        else if (ch == ' ')
        {
            // check if completion was called before space.
            if (!completions.empty())
            {
                msg = msg.substr(0, found+1) + completions[tabhit-1];
                // clear completion variable - job is done!.
                completions.clear();
            }

            addch(ch);
            msg.append((const char*)&ch);
        }
        else if (ch == KEY_UP)
        {
            if (uphit >= 0 && !history_.empty())
            {
                deleteToPosition(info_.size());
                mvprintw(row, (int)info_.size(), history_[uphit].c_str());     
                msg = history_[uphit];
                --uphit;
            }
        }
        else if (ch == KEY_DOWN)
        {
            if (uphit != (history_.size() -1))
            {
                ++uphit;
                deleteToPosition(info_.size());
                mvprintw(row, (int)info_.size(), history_[uphit].c_str());     
                msg = history_[uphit];
            }
        }
        else if (ch == KEY_LEFT)
        {
            getyx(stdscr, row, col);
            move(row, col-1);
        }
        else if (ch == KEY_RIGHT)
        {
            getyx(stdscr, row, col);
            move(row, col+1);
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
                deleteToPosition(info_.size()+found+1);
                mvprintw(row, info_.size()+found+1, completions[tabhit].c_str());
            }
            if (!completions.empty() && tabhit >= (int)completions.size())
            {
                tabhit = 0;
                deleteToPosition(info_.size()+found+1);
                mvprintw(row, info_.size()+found+1, completions[tabhit].c_str());
            }
            ++tabhit;
         }
         // enter key 
         else if (ch == 0x0a)
         {
                // do something with string processing.
             if (!msg.empty())
             {
                 bool isOk = sql_->parse(msg);
                 updateHistory(isOk, msg);
                
                 // reset to last history element.
                 uphit = history_.size()-1;

                 string out = sql_->result();
                 // print whatever it cames out: error  or result.
                 
                 getyx(stdscr, row, col);
                 ++row;
                 if (!out.empty())
                 {
                    // search for line termminator '\n' in output string and print to console. NOTE 
                    std::stringstream ss(out);
                    std::string line;
                    while ( std::getline(ss, line))
                    {
                        mvprintw(row, 0, line.c_str());
                        ++row;
                    }

                 }

             }
             
             getyx(stdscr, row, col);
             ++row;
             mvprintw(row, 0, info_.c_str());
             msg.clear();
         }
         // add clicked char to the screen and to the string which holds statement;
         else
         {
             addch(ch);
             msg.append((const char*)&ch);
         }

         refresh();
    } // end of while  

    return 0;
} // end of operator()

void Console::updateHistory(bool isOk, const string& msg)
{
     if (isOk)
     {
        // if parsing succeedes - save statement to history(it could be possible recall in the futurei with UP
        // and DOWN keys)
        // such statement will also be split into keywords for completion.
        
        istringstream iss(msg);
        std::copy(istream_iterator<string>(iss), istream_iterator<string>(), std::inserter(keywords_, keywords_.begin()));

     }

     // whenever was a result, remember last statement.
     history_.push_back(msg);

}

void Console::clearScreen()
{
    // clean all terminal 
    int r, c;
    getyx(stdscr, r, c);
    move(r, 0);
    
    while(r != 0)
    {
        deleteln(); move(--r, 0);
    }
    deleteln();
    mvprintw(0,0, info_.c_str());
    refresh();
}

void Console::deleteToPosition(int colnr)
{
    int c, r;
    getyx(stdscr, r, c);
    while(c > colnr)
    {
        mvdelch(r, --c);
    }
    refresh();
}

void Console::searchWord(const string& str, vector<string>& result)
{
    result.clear();

    if (str.empty())
        return; 

    std::set<string>::const_iterator cit;
    for (cit = keywords_.begin(); cit != keywords_.end(); ++cit)
    {
       string tmp = *cit;
       int res = tmp.compare(0, str.size(), str); 
       if (res == 0)
           result.push_back(tmp);
    }

}

