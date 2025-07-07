#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>

void SsdReadCmd::CheckAddressRange(long newAddress)
{
    if (newAddress < 0 || newAddress > 99) {
        std::ofstream outFile("ssd_output.txt");
        outFile << "ERROR" << std::endl;
        return;
    }
}