#include <vector>

#include "gmock/gmock.h"
#include "shellCmdParser.h"
#include "stdexcept"
#include "testShellCmdInterface.h"
#include "testShellCmdRead.h"
#include "testShellCmdWrite.h"

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
    TestShellReadCmd* convertedCmd = dynamic_cast<TestShellReadCmd*>(command);
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

TEST_F(ShellCmdParserFixture, WriteWithValidAddress) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command =
      cmdParser.getCommand({"write", "3", "0xAAAABBBB"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellWriteCmd*>(command));
  try {
    TestShellWriteCmd* convertedCmd = dynamic_cast<TestShellWriteCmd*>(command);
    EXPECT_EQ(3, convertedCmd->getAddress());
    EXPECT_EQ(0xAAAABBBB, convertedCmd->getData());
  } catch (std::exception& e) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress1) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command =
      cmdParser.getCommand({"write", "3", "AAAABBBB"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellErrorCmd*>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress2) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command =
      cmdParser.getCommand({"write", "0x333", "AAAABBBB"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellErrorCmd*>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress3) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command =
      cmdParser.getCommand({"write", "3", "0xAAAABBBB", "0xAAABBB"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellErrorCmd*>(command));
}

TEST_F(ShellCmdParserFixture, ExitCommand) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command = cmdParser.getCommand({"exit"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellExitCmd*>(command));
}

TEST_F(ShellCmdParserFixture, HelpCommand) {
  ShellCmdParser cmdParser;
  TestShellCmdInterface* command = cmdParser.getCommand({"help"});

  EXPECT_TRUE(nullptr != dynamic_cast<TestShellHelpCmd*>(command));
}
