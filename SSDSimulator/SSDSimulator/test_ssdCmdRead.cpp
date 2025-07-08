#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"

TEST(SSDRead, ReadExecutedWithoutError) {
    long addr = 19;
    long expectedData = 0x705ff43a;

    SsdReadCmd& readCmd = SsdReadCmd::getInstance();
    EXPECT_NO_THROW(readCmd.run());
}