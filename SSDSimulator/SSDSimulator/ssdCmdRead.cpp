#include "ssdCmdRead.h"

void SsdReadCmd::CheckAddressRange(long newAddress)
{
    if (newAddress < 0 || newAddress > 99) {
        throw std::invalid_argument("range�� ������ϴ�.");
    }
}