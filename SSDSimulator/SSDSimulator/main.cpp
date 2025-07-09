#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "ssdCmdParser.h"
#include "ssdInterface.h"

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
  auto command = parser.getCommand({argv + 1, argv + argc});
  
  newcommand = addBufferAndGetCmdToRun(command)
  command->run();
}
#endif