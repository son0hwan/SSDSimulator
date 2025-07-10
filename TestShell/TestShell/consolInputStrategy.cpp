#include "consoleInputStrategy.h"
#include <iostream>

bool ConsoleInputStrategy::hasNextCommand() {
    return true;
}

std::string ConsoleInputStrategy::getNextCommand() {
    std::string cmd;
    std::cout << "Shell> ";
    std::getline(std::cin, cmd);
    return cmd;
}