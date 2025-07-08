#include <vector>

#include "gmock/gmock.h"
#include "shellCmdParser.h"
#include "stdexcept"
#include "testShellCmdInterface.h"
#include "testShellCmdRead.cpp"
#include "testShellCmdWrite.cpp"

using namespace testing;
using namespace std;

class ShellCmdParserFixture : public Test {
 public:
};

TEST_F(ShellCmdParserFixture, ReadWithValidAddress) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command = cmdParser.getCommand({"read", "0"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellReadCmd*>(command));
  try {
    TestShellReadCmd* convertedCmd =
        dynamic_cast<TestShellReadCmd*>(command);
    EXPECT_EQ(0, convertedCmd->getAddress());
  } catch (std::exception& e) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, ReadWithInValidCommand) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command = cmdParser.getCommand({"READ", "3"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellErrorCmd*>(command));
}

TEST_F(ShellCmdParserFixture, ReadWithInValidAddress) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command = cmdParser.getCommand({"read", "AAA"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellErrorCmd*>(command));
}
