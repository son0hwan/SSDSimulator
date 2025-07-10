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

void ConsoleInputStrategy::print(const std::string& cmdName, const std::string& status) {
    std::cout << "[" << cmdName << "] " << status << std::endl << std::endl;
}