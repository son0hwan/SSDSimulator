#include "gmock/gmock.h"
#include "IOManager.h"
#include "ssdSimulator.h"
#include <vector>

using namespace testing;

TEST(BufferTest, FiveBuffersCreated) {
    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    ioManager.forceCreateFiveFreshBufferFiles();
    std::vector<std::string> created = ioManager.listBufferFiles();
    std::vector<std::string> expected = { "1_empty", "2_empty", "3_empty", "4_empty", "5_empty" };
    EXPECT_EQ(created, expected);
}