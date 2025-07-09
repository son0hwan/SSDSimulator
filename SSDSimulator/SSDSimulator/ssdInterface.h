#pragma once
#include "ssdSimulator.h"

class SsdCmdInterface {
 public:
  virtual void run() = 0;
  virtual bool isAllowBuffering(void) = 0;
};