#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "orasql.h"
#include <vector>
#include <string>
#include <set>

using std::string;

class Console 
{
    public:
        Console(); 
        ~Console();
        int exec();

    // private function/methods
    private:
        void searchWord(const string& str, vector<string>& results);
        void deleteToPosition(int colnr);
        void updateHistory(bool isOk, const string& msg);

    private:
        std::vector<string> history_;
        std::set<string> keywords_; 
        OraSql sql_;
};


#endif
