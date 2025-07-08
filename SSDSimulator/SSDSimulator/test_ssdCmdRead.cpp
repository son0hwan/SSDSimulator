#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>
#include <cstdint>

TEST(SSDRead, ReadExecutedWithoutError) {
    uint32_t addr = 19;
    long expectedData = 0x705ff43a;

    SsdReadCmd& readCmd = SsdReadCmd::getInstance();
    EXPECT_NO_THROW(readCmd.run());
}

TEST(SSDRead, ReadExecutedWithError) {
    SsdReadCmd& readCmd = SsdReadCmd::getInstance();
    readCmd.setAddress(100);
    readCmd.run();

    std::ifstream outFile("ssd_output.txt");
    ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt file open failed";

    std::string fileContent;
    std::getline(outFile, fileContent);
    EXPECT_EQ(fileContent, "ERROR");
}