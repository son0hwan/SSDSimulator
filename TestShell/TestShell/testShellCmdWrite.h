#pragma once
#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
 public:
  // 생성자는 parser에서 불러줄 것
  TestShellWriteCmd(long address, unsigned data)
      : address(address), data(data) {}

  void run() override {}

  long getAddress() { return address; }
  long getData() { return data; }

 private:
  const long address;
  const unsigned data;
};

class TestShellFullWriteCmd : public TestShellCmdInterface {
 public:
  // 생성자는 parser에서 불러줄 것
  TestShellFullWriteCmd() {}

  void run() override {}
};