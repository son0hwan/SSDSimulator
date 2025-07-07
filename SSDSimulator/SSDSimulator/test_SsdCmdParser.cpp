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
      SsdCmdInterface* GetSsdReadCmdParser(const std::string& command, const std::string& lba) {
        return cmdParser.parse(command, lba);
      }
};

TEST_F(SsdCmdParserFixture, ReadWithValidAddress) {
    SsdCmdInterface* parseResult = GetSsdReadCmdParser("READ", "3");
    EXPECT_TRUE(nullptr != dynamic_cast<SsdReadCmd*>(parseResult));

    try {
        SsdReadCmd* convertedCmd = dynamic_cast<SsdReadCmd*>(parseResult);
        EXPECT_EQ(3, convertedCmd->getAddress());
    } catch (std::exception& e){
        FAIL();
    }
}