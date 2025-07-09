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

	bool isError(const std::string& result) {
		return result == ERROR_STRING;
	}
	
	unsigned int rand() {
		return executor->rand();
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
		std::cout << " -. fullwrite [DATA_IN_HEX] \n";
		std::cout << " -. exit \n\n";
	}
};

class TestShellErrorCmd : public TestShellCmdInterface {
 public:
  TestShellErrorCmd() {}
  void run() override {
	  std::cout << "unknown command \n";
  }
};

class TestShellEraseCmd : public TestShellCmdInterface {
 public:
  TestShellEraseCmd(long address, long size) : address(address), size(size) {}
  virtual void run() override {}

  long getAddress() { return address; }
  long getSize() { return size; }
 protected:
  const long address;
  const long size;
};

class TestShellEraseRangeCmd : public TestShellEraseCmd {
 public:
  TestShellEraseRangeCmd(long startAddress, long endAddress)
      : TestShellEraseCmd(startAddress, (endAddress - startAddress + 1)) {}
  long getStartAddress() { return getAddress(); }
  long getEndAddress() { return getAddress() + getSize() - 1; }	
};

class TestShellScript4Cmd : public TestShellCmdInterface {
 public:
  TestShellScript4Cmd() {}
  void run() override {}
};

class TestShellFlushCmd : public TestShellCmdInterface {
 public:
  TestShellFlushCmd() {}
  void run() override {}
};