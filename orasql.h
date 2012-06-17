#ifndef _ORASQL_H
#define _ORASQL_H

#include "soci.h"
#include "oracle/soci-oracle.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <exception>
#include "interpreter.hpp"
#include <cstdarg>

#include <boost/algorithm/string.hpp>

using namespace soci;
using namespace std;

using example::interpreter;

void quit();

class Console;

class OraSql
{

public:
	OraSql(Console* console);
	~OraSql();
	
	bool connect(const string& server, const string& user, const string& passwd);
    
    // return results after parsing input: it could be sql statment or calling specific function.
    const string& result();
    bool parse(const string& str);

private: 
	bool statement(const string& str);
    void formatOutput(const vector<string>& column_names, const vector<vector<string> >& table); 

private:
	session sql_;
    interpreter interpreter_;
    string result_;

    Console* console_;
};


#endif
