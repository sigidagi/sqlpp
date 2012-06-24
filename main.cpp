#include "orasql.h"
#include "console.h"

using example::interpreter;

int main(int argc, char *argv[])
{

    Console console;
    
    // then connect to database.
    if (console.autologin())
        // loop for catching keyboard events
        return console.exec();
    else
        return 0;
    

}
