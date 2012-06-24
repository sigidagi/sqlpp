#ifndef _LOGIN_H
#define _LOGIN_H

#include "interpreter.hpp"
#include <string>

using std::string;

using example::interpreter;


class OraSql;

class Login
{
    public:
        Login(OraSql* sql);
        ~Login();
        
        void exec();
        bool succeeded();
        string prompt();

    private:
        bool connect(const string& str);

        OraSql* sql_;
        interpreter interpreter_;
    
        string prompt_;
        bool succees_;
        string newprompt_;
};


#endif
