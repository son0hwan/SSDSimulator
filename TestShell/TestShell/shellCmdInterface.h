#pragma once
#include <string>
#include <iostream>
#include "executor.h"
#include "common.h"

class shellCmdInterface {
public:
	virtual bool run() = 0;

	void setExecutor(Executor* executor) {
		this->executor = executor;
	}

	bool isError(const std::string& result) {
		return result == ERROR_STRING;
	}
	
	unsigned int rand() {
		return executor->rand();
	}
protected:
	Executor* executor = nullptr;
};