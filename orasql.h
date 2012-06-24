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
#include <cstdarg>

#include <boost/algorithm/string.hpp>

using namespace soci;
using namespace std;


class OraSql
{

public:
	OraSql();
	~OraSql();
	
	bool connect(const string& server, const string& user, const string& passwd);
    
	bool statement(const string& str);
    // return results after parsing input: it could be sql statment or calling specific function.
    const string& result();

private: 
    void formatOutput(const vector<string>& column_names, const vector<vector<string> >& table); 

private:
	session sql_;
    string result_;

};


#endif
