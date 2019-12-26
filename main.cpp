#include "program.h"

#include <iostream>

int main()
{
    CProgram Program;
    if(!Program.init())
    {
       std::cout << "startup failed, exitting" << std::endl;
       return -1;
    }
    Program.run();

    return 0;
}
