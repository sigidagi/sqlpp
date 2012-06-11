#include <QtCore/QCoreApplication>
#include "orasql.h"
#include <boost/thread.hpp>

using example::interpreter;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	interpreter inter;
	OraSql ora(inter);
    


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


	return a.exec();
}
