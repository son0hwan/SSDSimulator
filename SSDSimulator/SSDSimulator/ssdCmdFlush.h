#pragma once
#include "ssdInterface.h"

class SsdFlushCmd : public SsdCmdInterface {
public:
    static SsdFlushCmd& getInstance() {
        static SsdFlushCmd instance;
        return instance;
    }

    void run() override;
};
