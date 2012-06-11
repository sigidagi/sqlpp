#include "console.h"


void Console::operator() const
{

    std::string line = "nonempty";
    while (! line.empty())
    {
        std::cout << std::endl << "sqlpp> ", std::getline(std::cin,line);
        QString str = QString(line.c_str());
        emit consoleInput(str);
    }

}
