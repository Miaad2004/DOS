#include "dos_shell.h"
#include <iostream>
#include <string>

int main()
{
    DOSShell shell;
    std::string command;

    std::cout << "Mini-DOS Shell\n";
    std::cout << "Type 'exit' to quit\n\n";

    while (true)
    {
        std::cout << shell.getCurrentPath() << ">";
        std::getline(std::cin, command);

        if (command == "exit")
            break;

        shell.executeCommand(command);
    }

    return 0;
}