#include <vector>
#include "gmock/gmock.h"
#include "stdexcept"

using namespace testing;
using namespace std;

class ShellCmdParserFixture : public Test {
 public:

  void SetUp() override {
    std::streambuf* backup = std::cout.rdbuf();
    std::cout.set_rdbuf(localStream.rdbuf());
  }

  void TearDown() override { std::cout.set_rdbuf(backup); }

  string getPrintedString() const { return localStream.str(); }

  std::ostringstream localStream;
  std::streambuf* backup = nullptr;

};
