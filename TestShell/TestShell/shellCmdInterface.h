#pragma once
#include <string>
#include <iostream>
#include "executor.h"
#include "common.h"
#include "commandInputStrategy.h"

class shellCmdInterface {
public:
	virtual bool run() = 0;

	void runCmd(Executor* executor, CommandInputStrategy* inputStrategy) {
		setExecutor(executor);
		setInputStrategy(inputStrategy);
		prePrint();
		run();
	}
	void setExecutor(Executor* executor) {
		this->executor = executor;
	}
	void setInputStrategy(CommandInputStrategy* strategy) {
		inputStrategy = strategy;
	}

	bool isError(const std::string& result) {
		return result == ERROR_STRING;
	}
	
	unsigned int rand() {
		return executor->rand();
	}
	
	void printResult() {
		if (result == SUCCESS)
			printSuccess();
		else if (result == ERROR)
			printError();
		else if (result == SSDAccessFail)
			printSSDAccessFail();
	}
	void printSuccess() { inputStrategy->print(cmdName, SUCCESS); }
	void printError() { inputStrategy->print(cmdName, ERROR); }
	void printSSDAccessFail() { inputStrategy->print(cmdName, SSDAccessFail); }
	void prePrint() { inputStrategy->prePrint(cmdName); }
	bool isCmdSuccess() {
		if (result == SUCCESS)
			return true;
		else
			return false;
	}
	
protected:
	Executor* executor = nullptr;
	std::string cmdName ="unknown";
	CommandInputStrategy* inputStrategy;
	int result = SUCCESS;
};