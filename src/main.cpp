#include "dos_shell.h"
#include <iostream>
#include <string>

int main()
{
    DOSShell shell;
    std::string command;

    std::cout << "Mini-DOS Shell\n";
    std::cout << "Type 'exit' to quit\n\n";

    while (!shell.getShouldExit())
    {
        std::cout << shell.getCurrentPath() << ">";
        std::getline(std::cin, command);
        shell.executeCommand(command);
    }

    return 0;
}