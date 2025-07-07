#include "ssdInterface.h"
#include "ssdCmdRead.h"
#include "ssdCmdParser.h"	
#include<string>

SsdCmdInterface* SsdCmdParser::getCommand(const std::string& cmd, const std::string& arg1, const std::string& arg2)
{
	SsdReadCmd& readCmd = SsdReadCmd::getInstance();
	long address = std::stol(arg1);
	readCmd.setAddress(address);
	return &SsdReadCmd::getInstance();
}