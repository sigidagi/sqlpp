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

void quit()
{
    exit(0); 
}


class OraSql
{

public:
	OraSql(interpreter& inter) : interpreter_(inter) 
    {
        interpreter_.register_function("connect", &OraSql::connect, this);
        interpreter_.register_function("quit", &quit);
        interpreter_.register_function("exit", &quit);
    }

	~OraSql() { sql_.close(); }
	const session& sql() {return sql_; }
	bool connect(const string& server, const string& user, const string& passwd)
	{
		sql_.close();
		try {
			sql_.open(oracle, "service=" + server + " user=" + user + " password=" + passwd);
		}
		catch (oracle_soci_error const & e) {
			cerr << "Oracle error: " << e.err_num_ << " " << e.what() << endl;
			return false;
		}

		std::cout << "Connection succeeded!";
		return true;
	}
    


	bool statement(const string& str)
	{
		istringstream iss(str);
		vector<string> tokens;
		std::copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		vector<string>::const_iterator cit;

		if (tokens.empty())
			return false;
		// SELECT statment.
		else if (tokens[0] == string("select"))
		{
			cit = std::find(tokens.begin(), tokens.end(), string("from"));
			if (cit == tokens.end() || (++cit) == tokens.end())
				std::cerr << "Incomplete 'select' statement!" << endl;


			string ownertable = *(cit);
			string owner, table;
			size_t found = ownertable.find(".");
			if (found != string::npos)
			{
				owner = ownertable.substr(0, found);
				table = ownertable.substr(found+1);
				boost::to_upper(owner);
				boost::to_upper(table);
			}

			vector<string> column_names(16);
			vector<string> types(16);
			sql_ << "select column_name, data_type from all_tab_columns where owner = '" + owner + "' and table_name = '" + table + "'", into(column_names), into(types);

            
            // SD TODO:
			
		}
		else if (tokens[0] == string("update"))
		{

		}
		else if (tokens[0] == string("insert"))
		{

		}
		else
			return false;
    
        
        return true;
	}

private:
	session sql_;
    interpreter& interpreter_;
};


#endif
