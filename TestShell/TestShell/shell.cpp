#pragma once
#include "executor.h"
#include "shellCmdParser.h"
#include "shellCmdInterface.h"
#include "common.h"
#include "commandInputStrategy.h"
#include "consoleInputStrategy.h"

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

			exeCmd->runCmd(executor, inputStrategy);
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
private:
	Executor* executor;
	CommandInputStrategy* inputStrategy;
};