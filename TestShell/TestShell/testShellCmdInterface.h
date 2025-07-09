#pragma once
#include <string>
#include <iostream>
#include "executor.h"

class TestShellCmdInterface {
public:
	virtual void run() = 0;

	void setExecutor(Executor* executor) {
		this->executor = executor;
	}
protected:
	Executor* executor = nullptr;
};