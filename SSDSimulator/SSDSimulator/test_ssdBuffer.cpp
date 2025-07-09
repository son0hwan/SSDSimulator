#include "gmock/gmock.h"
#include "IOManager.h"
#include "ssdSimulator.h"
#include <vector>

#include "cmdBuffer.h"

using namespace testing;

TEST(BufferIOTest, FiveBuffersCreated) {
    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    bool created = ioManager.forceCreateFiveFreshBufferFiles();
    EXPECT_EQ(true, created);
    std::vector<std::string> createdBuffer = ioManager.getBufferFileList();
    std::vector<std::string> expectedBuffer = { "1_empty", "2_empty", "3_empty", "4_empty", "5_empty" };
    EXPECT_EQ(createdBuffer, expectedBuffer);
}

TEST(BufferIOTest, RenameBuffer) {
    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    std::vector<std::string> expectedBuffer = { "1_empty", "2_notempty", "3_empty", "4_notempty", "5_notempty" };
    bool created = ioManager.updateBufferFiles(expectedBuffer);
    EXPECT_EQ(true, created);
    std::vector<std::string> createdBuffer = ioManager.getBufferFileList();    
    EXPECT_EQ(createdBuffer, expectedBuffer);
}

TEST(BufferGetTest, RenameBuffer) {
    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    std::vector<std::string> expectedBuffer = { "1_W_1_0x12345678", "2_W_2_0x12345678", "3_E_3_2", "4_W_5_0x12345678", "5_E_6_10" };
    ioManager.updateBufferFiles(expectedBuffer);

    CommandBufferStorage storage;
    auto result = storage.getBufferFromStorage();
    EXPECT_EQ(result.size(), 5);
}