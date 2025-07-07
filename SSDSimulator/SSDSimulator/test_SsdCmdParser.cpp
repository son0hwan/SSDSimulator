#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdInterface.h"
#include "ssdCmdParser.h"

using namespace testing;

class SsdCmdParserFixture : public Test {
 public:
      SsdCmdParser cmdParser;
};



TEST_F(SsdCmdParserFixture, ReadWithValidAddress) {
    SsdCmdInterface* command = cmdParser.getCommand("READ", "3");
    EXPECT_TRUE(nullptr != dynamic_cast<SsdReadCmd*>(command));
    try {
        SsdReadCmd* convertedCmd = dynamic_cast<SsdReadCmd*>(command);
        EXPECT_EQ(3, convertedCmd->getAddress());
    } catch (std::exception& e){
        FAIL();
    }
}


}
  EXPECT_TRUE(nullptr != dynamic_cast<SsdReadCmd*>(result));

  try {
    SsdReadCmd* convertedCmd = dynamic_cast<SsdReadCmd*>(result);
    EXPECT_TRUE(3, convertedCmd->getAddress());
  } catch (std::exception e){
  
  }
}

TEST(SsdCmdParserFixture, WWithValidAddress) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("W", "2", "0x00000001");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdWriteCmd*>(result));

  try {
    SsdWriteCmd* convertedCmd = dynamic_cast<SsdWriteCmd*>(result);
    EXPECT_EQ(2, convertedCmd->getAddress());
    EXPECT_EQ(0x00000001, convertedCmd->getValue());
  } catch (std::exception e) {
      FAIL();
  }
}

TEST(SsdCmdParserFixture, InvalidCmd) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("BBB");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, ReadWithNonAddress) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("R", "BBB");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, WriteWithNonAddress) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("W", "BBB", "0x00000001");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, WriteWithNonData) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("W", "2", "BBB");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, ReadWithLessArg) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("R");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, ReadWithManyArg) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("R", "3", "BBB");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}
TEST(SsdCmdParserFixture, WriteWithLessArg) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("W", "2");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, WriteWithManyArg) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse("W", "2", "0x00000001", "BBB");

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}

TEST(SsdCmdParserFixture, NonArg) {
  SsdCmdParser cmdParser;
  SsdCmdInterface* result = cmdParser.parse();

  EXPECT_TRUE(nullptr != dynamic_cast<SsdErrorCmd*>(result));
}