#include "testShellCmd.h"
#include "common.h"

TestShellEraseRangeCmd::TestShellEraseRangeCmd(long startAddress, long endAddress)
	: TestShellEraseCmd(startAddress, (endAddress - startAddress + 1)) {
}
long TestShellEraseRangeCmd::getStartAddress() { return getAddress(); }
long TestShellEraseRangeCmd::getEndAddress() { return getAddress() + getSize() - 1; }