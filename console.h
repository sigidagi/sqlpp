#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <vector>
#include <string>
#include <set>

using std::string;

class OraSql;

class Console 
{
    public:
        Console(); 
        ~Console();
        int exec();

        // those bellow function can be called from orasql when statement(console input) is parsed.
        void clearScreen();

    // private function/methods
    private:
        void searchWord(const string& str, std::vector<string>& results);
        void deleteToPosition(int colnr);
        void updateHistory(bool isOk, const string& msg);

    private:
        std::vector<string> history_;
        std::set<string> keywords_; 
        OraSql* sql_;
    
        string info_;
};


#endif
