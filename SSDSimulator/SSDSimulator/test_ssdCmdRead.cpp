#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace testing;

class ReadTestFixture : public Test {
public:
    SsdReadCmd& readCmd = SsdReadCmd::getInstance();
    void runReadTest(uint32_t address) {
        readCmd.setAddress(address);
        readCmd.run();
    }

    void CheckOutputFileValid(const std::string& expectResult)
    {
        std::ifstream outFile(OUTPUT_FILENAME);
        ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt file open failed";

        std::string fileContent;
        std::getline(outFile, fileContent);
        EXPECT_EQ(fileContent, expectResult);
    }


protected:
    static const uint32_t VALID_ADDRESS = 19;
    static const uint32_t INVALID_ADDRESS = 100;
    static const uint32_t EXPECTED_DATA = 0x705ff43a;
    static const std::string OUTPUT_ERROR;
    static const std::string OUTPUT_VALID_READ;
    static const std::string OUTPUT_FILENAME;
    static const std::string NAND_FILENAME;
};

const std::string ReadTestFixture::OUTPUT_ERROR = "ERROR";
const std::string ReadTestFixture::OUTPUT_FILENAME = "ssd_output.txt";
const std::string ReadTestFixture::OUTPUT_VALID_READ = "0x705FF43A";
const std::string ReadTestFixture::NAND_FILENAME = "ssd_nand.txt";

TEST_F(ReadTestFixture, ReadExecutedWithoutError) {
    EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
}

TEST_F(ReadTestFixture, ReadExecutedWithError) {
    runReadTest(INVALID_ADDRESS);

    CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(ReadTestFixture, ReadValidData) {
    EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
    EXPECT_EQ(readCmd.getReadData(), EXPECTED_DATA);

    CheckOutputFileValid(OUTPUT_VALID_READ);
}