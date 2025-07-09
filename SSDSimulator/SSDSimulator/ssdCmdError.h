#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdErrorCmd : public SsdCmdInterface {
public:
	void run() override;
	BufferedCmdInfo* getBufferedCmdInfo(void) {
		return nullptr;
	}
};