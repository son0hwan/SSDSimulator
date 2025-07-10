#pragma once
#include "ssdInterface.h"

class SsdFlushCmd : public SsdCmdInterface, public std::enable_shared_from_this<SsdFlushCmd> {
public:
    SsdFlushCmd() = default;
    void run() override;
    BufferedCmdInfo* getBufferedCmdInfo(void) override;

};