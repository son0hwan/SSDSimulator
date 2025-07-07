#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdInterface.h"
#include "ssdCmdParser.h"

using namespace testing;

class SsdCmdParserFixture : public Test {
 public:

private:
};

TEST_F(SsdCmdParserFixture, ReadWithValidAddress) {
      SsdCmdParser cmdParser;
      SsdCmdInterface* result = cmdParser.parse("READ", "3");

      EXPECT_TRUE(nullptr != dynamic_cast<SsdReadCmd*>(result));

      try {
        SsdReadCmd* convertedCmd = dynamic_cast<SsdReadCmd*>(result);
        EXPECT_EQ(3, convertedCmd->getAddress());
      } catch (std::exception& e){
          FAIL();
      }
}