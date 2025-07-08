#pragma once
#include "testShellCmdInterface.h"

class TestShellReadCmd : public TestShellCmdInterface {
 public:
  // 생성자는 parser에서 불러줄 것
  TestShellReadCmd(long address) : address(address) {}

  void run() override {}

  long getAddress() { return address; }

 private:
  const long address;
};

class TestShellFullReadCmd : public TestShellCmdInterface {
 public:
  // 생성자는 parser에서 불러줄 것
  TestShellFullReadCmd() {}

  void run() override {}
};