#pragma once
#include <cstdint>
#include "ssdSimulator.h"

class SsdCmdInterface {
public:
    virtual void run() = 0;
};