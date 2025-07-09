#include "testShellCmd.h"
#include "common.h"

TestShellEraseCmd::TestShellEraseCmd(long address, long size) : address(address), size(size) {}
bool TestShellEraseCmd::run() { return true; }

long TestShellEraseCmd::getAddress() { return address; }
long TestShellEraseCmd::getSize() { return size; }