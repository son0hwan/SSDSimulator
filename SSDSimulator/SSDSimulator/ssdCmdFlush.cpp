#include "ssdCmdFlush.h"
#include "bufferedCmdInfo.h"

void SsdFlushCmd::run() {
    //TODO: Adding flush 
}

BufferedCmdInfo* SsdFlushCmd::getBufferedCmdInfo(void) {
	return new BufferedCmdInfo(this);
}


