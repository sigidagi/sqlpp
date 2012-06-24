#include "login.h"
#include "orasql.h"
#include "utils.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

Login::Login(OraSql* sql) : sql_(sql), succees_(false)
{

    interpreter_.register_function("connect", &OraSql::connect, sql_);
}

Login::~Login()
{

}

void Login::exec()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    int ch;
    
    vector<string> prompts;
    prompts.push_back("server: ");
    prompts.push_back("user:   ");
    prompts.push_back("passwd: ");

    vector<string> answers;

    int hits(0);
    prompt_ = prompts[hits];
    
    sqlpp::clearScreen(stdscr);
    mvprintw(0, 0, prompt_.c_str());
    refresh();
    
    string msg_;

    bool keepRunning = true;
    int attempts(0);

    while(keepRunning)
    {
        ch = getch();
        // enter
        if (ch == '\n' || ch == KEY_ENTER)
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
                
                if (connect(msg))
                {
                    newprompt_ = answers[1]+"@"+answers[0]+"> ";
                    msg_.clear();
                    succees_ = true;
                    keepRunning = false;

                }
                else
                {
                    int row, col;
                    getyx(stdscr, row, col);
                    ++row;
                    mvprintw(row, 0, "try gain, or press Ctr-C to quit.");
                
                    ++attempts;

                    if (attempts != 3)
                    {
                        msg_.clear();
                        answers.clear();
                        hits = 0;
                        prompt_ = prompts[hits];
                        ++row;
                        mvprintw(row, 0, prompt_.c_str());
                    }
                    else
                    {
                        ++row;
                        mvprintw(row, 0, "that's enough! I'm tired of you. bey bey");
                        succees_ = false;
                        keepRunning = false;
                    }

                }
                
            }
        } // if KEY_ENTER
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
        else if (ch == KEY_BACKSPACE) 
        {
            sqlpp::deleteChar(stdscr, prompt_, msg_);
        }
        else 
            sqlpp::insertChar(stdscr, prompt_, msg_, ch);

    
        refresh();

    }
    
    endwin();
} // exec function.

bool Login::connect(const string& msg)
{
    try                          
    {
        interpreter_.parse_input(msg);
    }
    catch (std::runtime_error &error) 
    { 
        return false;
    }

    return true;
}

bool Login::succeeded()
{
    return succees_;
}

string Login::prompt()
{
    return newprompt_;
}
