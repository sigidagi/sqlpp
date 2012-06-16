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


class OraSql
{

public:
	OraSql() 
    {
        interpreter_.register_function("connect", &OraSql::connect, this);
        interpreter_.register_function("quit", &quit);
        interpreter_.register_function("exit", &quit);
    }

	~OraSql() { sql_.close(); }
	const session& sql() {return sql_; }


	bool connect(const string& server, const string& user, const string& passwd)
	{
        stringstream ss;
		sql_.close();
		try {
			sql_.open(oracle, "service=" + server + " user=" + user + " password=" + passwd);
		}
		catch (oracle_soci_error const & e) {
			ss << "Oracle error: " << e.err_num_ << " " << e.what(); 
            result_ = ss.str();
			return false;
		}

		ss << "Connection succeeded!";
        result_ = ss.str();
		return true;
	}
    
    // return results after parsing input: it could be sql statment or calling specific function.
    const string& result();
    bool parse(const string& str);

private: 
	bool statement(const string& str);

private:
	session sql_;
    interpreter interpreter_;
    string result_;
};


#endif
