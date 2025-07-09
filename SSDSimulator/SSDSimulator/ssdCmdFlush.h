#pragma once
#include "ssdInterface.h"

class SsdFlushCmd : public SsdCmdInterface {
public:
    SsdFlushCmd() = default;
    void run() override;
    bool isAllowBuffering() override { return false; }

};