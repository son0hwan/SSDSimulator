#include "ssdInterface.h"
#include "ssdCmdRead.h"
#include "ssdCmdParser.h"	



SsdCmdInterface* SsdCmdParser::parse(const std::string& cmd, const std::string& arg1, const std::string& arg2)
{
	return new SsdReadCmd{3};
}