#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>

void SsdReadCmd::CheckAddressRange(uint32_t newAddress)
{
    if (newAddress < 0 || newAddress > 99) {
        std::ofstream outFile("ssd_output.txt");
        outFile << "ERROR" << std::endl;
        return;
    }
}