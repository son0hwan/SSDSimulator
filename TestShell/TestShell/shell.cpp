#pragma once
#include "executor.h"
#include "shellCmdParser.h"
#include "shellCmdInterface.h"
#include "common.h"
#include "commandInputStrategy.h"
#include "consoleInputStrategy.h"

#define MOCK_TEST

#ifdef MOCK_TEST
#include <iostream>
#include <fstream>
#endif

class TestShell {
public:
	TestShell(Executor* executor) : executor{ executor }, inputStrategy{ new ConsoleInputStrategy{} } {}
	TestShell(Executor* executor, CommandInputStrategy* strategy) : executor{ executor }, inputStrategy{ strategy } {}

	void run() {
		Logger::getInstance().initLogFile();

		while (inputStrategy->hasNextCommand()) {
			ShellCmdParser shellCmdParser;
			std::string cmd = inputStrategy->getNextCommand();
			if (cmd.empty()) continue;

			LOG(cmd);

			std::vector<std::string> token = splitBySpace(cmd);

			std::shared_ptr<shellCmdInterface> sharedCmd = shellCmdParser.getCommand(token);
			shellCmdInterface* exeCmd = sharedCmd.get();
			
			if (exeCmd == nullptr) break;

			exeCmd->setExecutor(executor);
			exeCmd->setInputStrategy(inputStrategy);
			exeCmd->prePrint();
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
		std::shared_ptr<shellCmdInterface> sharedCmd = shellCmdParser.getCommand(token);
		shellCmdInterface* exeCmd = sharedCmd.get();

		if (exeCmd == nullptr) return;

		exeCmd->setExecutor(executor);
		exeCmd->setInputStrategy(inputStrategy);
		exeCmd->run();
	}
#endif
private:
	Executor* executor;
	CommandInputStrategy* inputStrategy;
};