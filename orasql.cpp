#include "orasql.h"
#include "console.h"
#include <boost/bind.hpp>


void quit()
{
    exit(0);
}

OraSql::OraSql(Console* console) : console_(console)
{
    interpreter_.register_function("connect", &OraSql::connect, this);
    interpreter_.register_function("clear", &Console::clearScreen, console_);
    interpreter_.register_function("quit", &quit);
    interpreter_.register_function("exit", &quit);
}

OraSql::~OraSql()
{
    sql_.close();
}

bool OraSql::connect(const string& server, const string& user, const string& passwd)
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

bool OraSql::parse(const string& line)
{
    stringstream ss;
    result_.clear();

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
    vector<string>::iterator cit;

    if (tokens.empty())
    {
        return false;
    }
    // SELECT statment.
    else if (tokens[0] == string("select"))
    {
        cit = std::find(tokens.begin(), tokens.end(), string("from"));
        if (cit == tokens.end() || (cit+1) == tokens.end())
        {
            result_ = "Incomplete 'select' statement!";
            return false;
        }

    
        string ownertable = *(cit+1);
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


        
        // select column names. separate consdition is '*'
        vector<string> selectedNames;
        std::copy(tokens.begin()+1, cit, std::back_inserter(selectedNames));
        if (selectedNames.size() == 1 && selectedNames[0] == "*" ) 
        {

            vector< vector<string> > results(column_names.size(), vector<string>(100));
            for (size_t col = 0; col < results.size(); ++col)
            {
                sql_ << "select " + column_names[col] + " from " + ownertable, into(results[col]); 
            }
        
       
            formatOutput(column_names, results);
            return true;

        }
        else if (selectedNames.size() > 0)
        {
            vector< vector<string> > results(selectedNames.size(), vector<string>(100));
            for (size_t col = 0; col < results.size(); ++col)
            {
                sql_ << "select " + selectedNames[col] + " from " + ownertable, into(results[col]); 
            }
        
       
            formatOutput(selectedNames, results);
            return true;
        }

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

void OraSql::formatOutput(const vector<string>& column_names, const vector<vector<string> >& table)
{
    
    result_ = "\n"; 

    vector<int> pads;
    size_t col(0);
    for (col = 0; col < table.size(); ++col)
    {

        vector<int> sizes;
        std::transform(table[col].begin(), table[col].end(), std::back_inserter(sizes), boost::bind(&string::size, _1));
        int mlength = *std::max_element(sizes.begin(), sizes.end());
        if ((int)column_names[col].size() > mlength)
            mlength = (int)column_names[col].size();
        
        pads.push_back(mlength+1);
    }
    
    for (col = 0; col < column_names.size(); ++col)
    {
        result_ += column_names[col] + string(pads[col] - (int)column_names[col].size(), ' ');
    }
 
    result_ += "\n";
    for (col = 0; col < column_names.size(); ++col)
    {
        result_ += string(pads[col], '-');
    }
    
    result_ += "\n";
    int row(0);
    
    for (row = 0; row < (int)table[0].size(); ++row)
    {
        for (col = 0; col < table.size(); ++col)
            result_ += table[col][row] + string(pads[col] - (int)table[col][row].size(), ' ');

        result_ += "\n";
    }
    result_ += "\n";

}
