#include "testShellCmd.h"
#include "common.h"

TestShellEraseCmd::TestShellEraseCmd(long address, long size) : address(address), size(size) {}
void TestShellEraseCmd::run() {}

long TestShellEraseCmd::getAddress() { return address; }
long TestShellEraseCmd::getSize() { return size; }