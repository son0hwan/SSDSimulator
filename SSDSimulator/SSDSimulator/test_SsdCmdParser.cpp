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