#pragma once
#include "ssdSimulator.h"

class BufferedCmdInfo;

class SsdCmdInterface {
 public:
  virtual void run() = 0;
  virtual BufferedCmdInfo* getBufferedCmdInfo(void) = 0;
};