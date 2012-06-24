#include "orasql.h"
#include "login.h"
#include "console.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using example::interpreter;

int main(int argc, char *argv[])
{
   
    boost::shared_ptr<OraSql> sql = boost::make_shared<OraSql>();    
   
    Login login(sql.get());
    login.exec();

    if (!login.succeeded())
       return 0; 
        
    string prompt = login.prompt();

    Console console(sql.get(), prompt);
    
    return console.exec();

}
