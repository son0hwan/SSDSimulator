#pragma once
#include "executor.h"
#include "shellCmdParser.h"
#include "testShellCmdInterface.h"
#include "common.h"

#define MOCK_TEST

#ifdef MOCK_TEST
#include <iostream>
#include <fstream>
#endif

class TestShell {
public:
	TestShell(Executor* executor) : executor { executor } {}

	void run() {
		while (true) {
			std::string cmd;
			ShellCmdParser shellCmdParser;

			std::cout << "Shell> ";
			std::getline(std::cin, cmd);

			if (cmd.empty()) continue;

			std::vector<std::string> token = splitBySpace(cmd);
			TestShellCmdInterface* exeCmd = shellCmdParser.getCommand(token);
			
			if (exeCmd == nullptr) break;
			
			exeCmd->setExecutor(executor);
			exeCmd->run();	
		}
	}

	std::vector<std::string> splitBySpace(const std::string& str) {
		std::istringstream iss(str);
		std::vector<std::string> tokens;
		std::string word;

		while (iss >> word) {
			tokens.push_back(word);
		}

		return tokens;
	}

#ifdef MOCK_TEST
	void fake_command(std::string cmd) {
		ShellCmdParser shellCmdParser;
		std::vector<std::string> token = splitBySpace(cmd);
		TestShellCmdInterface* exeCmd = shellCmdParser.getCommand(token);

		if (exeCmd == nullptr) return;

		exeCmd->setExecutor(executor);
		exeCmd->run();
	}

	void writeToOutputFile(std::string contents) {
		std::ofstream file(OUTPUT_FILE_NAME);
		if (!file.is_open()) {
			std::cerr << "fail to open file";
			return;
		}
		file << contents;  
		file.close();     
	}
#endif
private:
	Executor* executor;
};