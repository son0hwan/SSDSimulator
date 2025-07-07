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
    void runReadTest(long address) {
        readCmd.setAddress(address);
        readCmd.run();
    }

    void deleteFileIfExists(const std::string& filename) {
        if (std::ifstream(filename)) {
            std::remove(filename.c_str());  // 기존 파일 삭제
        }
    }

protected:
    static const long VALID_ADDRESS = 19;
    static const long INVALID_ADDRESS = 100;
    static const long EXPECTED_DATA = 0x705ff43a;
    static const std::string OUTPUT_ERROR;
    static const std::string OUTPUT_FILENAME;
    static const std::string NAND_FILENAME;
};

const std::string ReadTestFixture::OUTPUT_ERROR = "ERROR";
const std::string ReadTestFixture::OUTPUT_FILENAME = "ssd_output.txt";
const std::string ReadTestFixture::NAND_FILENAME = "ssd_nand.txt";


TEST_F(ReadTestFixture, ReadExecutedWithoutError) {
    EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
}

TEST_F(ReadTestFixture, ReadExecutedWithError) {
    runReadTest(INVALID_ADDRESS);

    std::ifstream outFile(OUTPUT_FILENAME);
    ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt 파일 열기 실패";

    std::string fileContent;
    std::getline(outFile, fileContent);
    EXPECT_EQ(fileContent, OUTPUT_ERROR);
}

TEST_F(ReadTestFixture, ReadValidData) {
    EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
    EXPECT_EQ(readCmd.getReadData(), EXPECTED_DATA);
}
