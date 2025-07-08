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
	Executor* executor;
};

constexpr TestShellCmdInterface* TEST_SHELL_EXIT_CMD = nullptr;

class TestShellInvalidCmd : public TestShellCmdInterface {
 public:
  TestShellInvalidCmd() {}

  void run() override {}
};

class TestShellHelpCmd : public TestShellCmdInterface {
 public:
	TestShellHelpCmd() {}

	void run() override {
		std::cout << "\n [HELP]\n";
		std::cout << " -. read [LBA] \n";
		std::cout << " -. write [LBA] [DATA_IN_HEX] \n";
		std::cout << " -. fullread \n";
		std::cout << " -. fullwrite \n";
		std::cout << " -. exit \n\n";
	}
};

class TestShellTestScript1Cmd : public TestShellCmdInterface {
 public:
  TestShellTestScript1Cmd() {}
  void run() override {}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
 public:
  TestShellScript2Cmd() {}
  void run() override {}
};

class TestShellErrorCmd : public TestShellCmdInterface {
 public:
  TestShellErrorCmd() {}
  void run() override {
	  std::cout << "unknown command \n";
  }
};