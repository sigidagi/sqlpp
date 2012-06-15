#include "orasql.h"
#include "console.h"

using example::interpreter;

int main(int argc, char *argv[])
{

    Console console;
    console();


        /*
		 *try                          
		 *{
		 *    interpreter.parse_input(line);
		 *}
		 *catch (example::NotAFunction& error)
		 *{
		 *    ora.statement(line);
		 *    //std::cout << error.what() << std::endl;
		 *}
		 *catch (std::runtime_error &error) 
		 *{ 
		 *    std::cerr << error.what() << std::endl; 
		 *}
         */


	return 0; 
}
