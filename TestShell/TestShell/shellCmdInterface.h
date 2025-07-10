#pragma once
#include <string>
#include <iostream>
#include "executor.h"
#include "common.h"
#include "commandInputStrategy.h"

class shellCmdInterface {
public:
	virtual bool run() = 0;

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
	
	void printSuccess() { inputStrategy->print(cmdName, "Done"); }
	void printError() { inputStrategy->print(cmdName, "Error"); }
	
protected:
	Executor* executor = nullptr;
	std::string cmdName ="unknown";
	CommandInputStrategy* inputStrategy;
};