#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "ssdCmdParser.h"
#include "ssdInterface.h"
#include "cmdBuffer.h"

#ifdef _DEBUG
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
using std::string;
using std::vector;

int main(int argc, char* argv[]) {
	SsdCmdParser parser;
	auto parsedCmd = parser.getCommand({ argv + 1, argv + argc });

	auto commandList = CommandBuffer::getInstance().addBufferAndGetCmdToRun(parsedCmd.get());
	for (SsdCmdInterface* command : commandList) {
		command->run();
	}
}
#endif