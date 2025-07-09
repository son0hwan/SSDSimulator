#pragma once
#include "ssdInterface.h"

class SsdFlushCmd : public SsdCmdInterface {
public:
    SsdFlushCmd() = default;
    void run() override;
    BufferedCmdInfo* getBufferedCmdInfo(void) override;

};