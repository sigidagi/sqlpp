#include "console.h"
#include "orasql.h"
#include <curses.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using std::string;

Console::Console() : sql_(0), prompt_("sqlpp>")
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    sql_ = new OraSql(this);
    //screenbuffer_.resize(LINES);
}

Console::~Console()
{
    endwin();
    if (sql_ != 0)
    {
        delete sql_;
        sql_ = 0;
    }
}

bool Console::autologin()
{
    string msg = "connect orclas sigis sd"; // + answers[0] + " " + answers[1] + " " + answers[2];
    prompt_ = "sqlpp> ";
    return true;
}

bool Console::login()
{
    char ch;
    keypad(stdscr, TRUE);
    
    vector<string> prompts;
    prompts.push_back("server: ");
    prompts.push_back("user: ");
    prompts.push_back("passwd: ");

    vector<string> answers;

    int hits(0);
    prompt_ = prompts[hits];
    
    clearScreen();
    mvprintw(0, 0, prompt_.c_str());
    refresh();

    while(1)
    {
        ch = getch();
        // enter
        if (ch == '\n')
        {
            if (answers.size() != 3) 
            {
                answers.push_back(msg_);
                msg_.clear();
                if (hits != 2) 
                {
                    prompt_ = prompts[++hits];
                    int row, col;
                    getyx(stdscr, row, col);
                    ++row;
                    mvprintw(row, 0, prompt_.c_str());
                }


            }
            if (answers.size() == 3)
            {
                string msg = "connect " + answers[0] + " " + answers[1] + " " + answers[2];
                bool isOk  = sql_->parse(msg);                
                if (isOk)
                {
                    prompt_ = answers[1]+"@"+answers[0]+"> ";
                    msg_.clear();
                    return true;
                }
                else
                {
                    //printResults(sql_->result());        
                    int row, col;
                    getyx(stdscr, row, col);
                    ++row;
                    mvprintw(row, 0, "try gain, or press Ctr-C to quit.");
               

                    msg_.clear();
                    answers.clear();
                    hits = 0;
                    prompt_ = prompts[hits];
                    ++row;
                    mvprintw(row, 0, prompt_.c_str());
                }
            }
        }
        else if (ch == KEY_LEFT)
        {
            int row, col;
            getyx(stdscr, row, col);
            if (col > (int)prompt_.size())
                move(row, col-1);
        }
        else if (ch == KEY_RIGHT)
        {
            int row, col;
            getyx(stdscr, row, col);
            if (col < ((int)msg_.size() + (int)prompt_.size()))
                move(row, col+1);
        }
        // backspace or "delete char before cursor."
        else if (ch == '\a')
        {
            deleteChar();
        }
        else 
            insertChar(ch);
    
        refresh();

    }
}

int Console::exec() 
{
   int ch;   
   // current row and column - defines cursor position.
   int row(0), col(0);

   // keeps track how many times tab key was pressed (up key was pressed)
   // based on this parameters completion and history is implemented.
   int tabhit(0), uphit(0);
   size_t found(0); 

   string word;
   // only those keywords which fits typed word.
   vector<string> completions;

   clearScreen();
   screenbuffer_.push_back(prompt_);
   mvprintw(0,0,prompt_.c_str());
   refresh();

   while(1)
   {
        ch = getch();
        if (ch == KEY_BACKSPACE ) // || ch == 0x08)
        //if (ch == '\a')
        {
            deleteChar();
        }
        else if (ch == KEY_UP)
        {
            if (uphit >= 0 && !history_.empty())
            {
                deleteToPosition(prompt_.size());

                getyx(stdscr, row, col);
                mvprintw(row, (int)prompt_.size(), history_[uphit].c_str());     
                msg_ = history_[uphit];
                --uphit;
            }
        }
        else if (ch == KEY_DOWN)
        {
            if (uphit != (history_.size() -1))
            {
                ++uphit;
                deleteToPosition(prompt_.size());

                getyx(stdscr, row, col);
                mvprintw(row, (int)prompt_.size(), history_[uphit].c_str());     
                msg_ = history_[uphit];
            }
        }
        else if (ch == KEY_LEFT)
        {
            getyx(stdscr, row, col);
            if (col > (int)prompt_.size())
                move(row, col-1);
        }
        else if (ch == KEY_RIGHT)
        {
            getyx(stdscr, row, col);
            if (col < ((int)msg_.size() + (int)prompt_.size()))
                move(row, col+1);
        }
        // tab key == 0x09
        else if (ch == 0x09)
        {
            // delete char '\t'
            delch();
            // do some completion.
            found = msg_.rfind(" ");
            if (found != string::npos)
                word = msg_.substr(found+1); 
            else
                word = msg_;

            searchWord(word, completions);
            
            if (!completions.empty())
            {
                deleteToPosition(prompt_.size()+found+1);
                mvprintw(row, prompt_.size()+found+1, completions[tabhit].c_str());
                ++tabhit;
                if (tabhit == (int)completions.size()) tabhit = 0;
            }
         }
         else if (ch == ' ')
         {
             // check if completion was called before space.
             if (!completions.empty())
             {
                 if (tabhit == 0)
                    msg_ = msg_.substr(0, found+1) + completions[completions.size()-1];
                 else
                    msg_ = msg_.substr(0, found+1) + completions[tabhit-1];
             }

             // clear completion variable - job is done!.

             completions.clear();
             tabhit = 0;
           
             insertChar(ch);
         }

         // enter key 
         else if (ch == '\n' || ch == KEY_ENTER)
         {
             getyx(stdscr, row, col);

             screenbuffer_.push_back(prompt_ + msg_);
             int nlines(0);
                // do something with string processing.
             if (!msg_.empty())
             {
                 bool isOk = sql_->parse(msg_);
                 // whenever was a result, remember last statement.
                 history_.push_back(msg_);
                 if (isOk) updateKeywords(msg_);
                
                 // reset to last history element.
                 uphit = history_.size()-1;

                 string out = sql_->result();
                 // print whatever it cames out: error  or result.
                
                 nlines = pushToBuffer(out);
                 if ((row + nlines) > LINES-2)
                    repaintScreen();
                 else
                 {
                    ++row;
                    int startpos = (int)screenbuffer_.size() - nlines;
                    for (int k = 0; k < nlines; ++k, ++row)
                        mvprintw(row, 0, screenbuffer_[startpos+k].c_str());
                   
                    mvprintw(row, 0, prompt_.c_str());
                 }
             // and if user just pressed "enter" without entring anything 
             }
             else
             {
                 if (row > LINES-2)
                     repaintScreen();
                 else
                 {
                     ++row;
                     mvprintw(row, 0, prompt_.c_str());
                 }

             }

             // prepare to start new statment. 
             msg_.clear();
         }
         else
         {
             // inserts or appends char to the message strig and shows on console. 
             insertChar(ch);
         }

         refresh();
    } // end of while  

    return 0;
} // end of operator()



void Console::repaintScreen()
{
    for (int i = 0; i < (screenbuffer_.size() - LINES+1); ++i)
        screenbuffer_.pop_front();

    clearScreen(); 
    
    int row, col;
    getyx(stdscr, row, col);
    for (int k = 0; k < (int)screenbuffer_.size(); ++k, ++row)
        mvprintw(row, 0, screenbuffer_[k].c_str());
   
    mvprintw(row, 0, prompt_.c_str());
//    refresh();
}

// return number of lines required for the output
int Console::pushToBuffer(const string& out)
{
     int nlines(0);
     if (out.empty())
         return nlines;

    // search for line termminator '\n' in output string and print to console. NOTE 
    std::stringstream ss(out);
    std::string line;
    while ( std::getline(ss, line))
    {
        screenbuffer_.push_back(line);
        ++nlines;
    }

    return nlines;
}

void Console::deleteChar()
{
    int row, col;
    getyx(stdscr, row, col); 
    if (col > (int)prompt_.size())
    {
        int pos = col - (int)prompt_.size() - 1;
        msg_.erase(msg_.begin() + pos);
        
        deleteToPosition(0);
        string line = prompt_ + msg_;
        mvprintw(row, 0, line.c_str()); 
        move(row, --col);
    }
     
}

void Console::insertChar(char ch)
{
     int row, col;
     getyx(stdscr, row, col);
     msg_.insert(col - (int)prompt_.size(), 1, ch);
     
     deleteToPosition(0);
     string line = prompt_ + msg_;
     mvprintw(row, 0, line.c_str()); 
     move(row, ++col);

}

void Console::updateKeywords(const string& str)
{
    // if parsing succeedes - save statement to history(it could be possible recall in the futurei with UP
    // and DOWN keys)
    // such statement will also be split into keywords for completion.
    
    istringstream iss(str);
    std::copy(istream_iterator<string>(iss), istream_iterator<string>(), std::inserter(keywords_, keywords_.begin()));
}

void Console::quit()
{
    endwin();
    exit(0);
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

