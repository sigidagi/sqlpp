#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <vector>
#include <string>
#include <deque>
#include <set>

using std::string;

class OraSql;

class Console 
{
    public:
        Console(); 
        ~Console();
        int exec();
        bool login();
        bool autologin();

        // those bellow function can be called from orasql when statement(console input) is parsed.
        void clearScreen();
        void quit();

    // private function/methods
    private:
        void insertChar(char ch);
        void deleteChar();
        void searchWord(const string& str, std::vector<string>& results);
        void deleteToPosition(int colnr);
        void updateKeywords(const string& str);
        int pushToBuffer(const string& out);
        void repaintScreen();

    private:
        std::vector<string> history_;
        std::set<string> keywords_; 
        std::deque<string> screenbuffer_; 
        
        OraSql* sql_;
    
        string prompt_;
        string msg_;
};


#endif
