#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "../Head/CmdHandler.h"

void init()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void run_cli()
{
    std::string arg;
    while (std::getline(std::cin, arg))
    {
        CmdHandler::execute(arg);
    }
}

int main()
{
    init();
    run_cli();
    return 0;
}
