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
        void operator()();

    // private function/methods
    private:
        void searchWord(const string& str, vector<string>& results);
        void deleteToPosition(int colnr);

    private:
        std::vector<string> history_;
        std::set<string> keywowds_; 
        OraSql sql_;
};


#endif
