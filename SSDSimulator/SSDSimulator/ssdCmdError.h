#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdErrorCmd : public SsdCmdInterface {
 public:
  void run() override;
  bool isAllowBuffering() override { return false; }
};