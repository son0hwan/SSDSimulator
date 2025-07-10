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

void ConsoleInputStrategy::print(const std::string& cmdName, int status) {
    std::string statusStr;
    if (status == SUCCESS)
        statusStr = "Done";
    else if (status == ERROR)
        statusStr = "Error";
    else if (status == SSDAccessFail)
        statusStr = "FAIL to execute ssd.exe";
    else
        return;

    std::cout << "[" << cmdName << "] " << statusStr << std::endl << std::endl;
}

void ConsoleInputStrategy::prePrint(const std::string& cmdName) {
    return;
}