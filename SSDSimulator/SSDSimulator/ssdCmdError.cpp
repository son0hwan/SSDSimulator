#include "ssdCmdError.h"

void SsdErrorCmd::run() {
    SsdSimulator::getInstance().getIoManager().output().updateError();
}
