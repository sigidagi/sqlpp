#include "orasql.h"

void quit()
{
    exit(0);
}

bool OraSql::parse(const string& line)
{
    stringstream ss;

    try                          
    {
        interpreter_.parse_input(line);
        // it bellow function do not throw exception, it calls specified function
        // which should return results as string.
        return true;
    }
    catch (example::NotAFunction& error)
    {
        return statement(line);
    }
    catch (std::runtime_error &error) 
    { 
        ss  << error.what() << std::endl; 
        result_ = ss.str();
        return false;
    }

    return false;

}

const string& OraSql::result()
{
    return result_;
}


bool OraSql::statement(const string& str)
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
            {
			    result_ = "Incomplete 'select' statement!";
                return false;
            }


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
