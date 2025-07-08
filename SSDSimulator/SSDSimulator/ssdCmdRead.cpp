#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

const std::string SsdReadCmd::OUTPUT_FILENAME = "ssd_output.txt";
const std::string SsdReadCmd::OUTPUT_ERROR = "ERROR";

void SsdReadCmd::CheckAddressRange(uint32_t address)
{
	if (address < 0 || address > 99) {
		std::ofstream outFile(OUTPUT_FILENAME);
		outFile << OUTPUT_ERROR << std::endl;
		return;
	}
}

void SsdReadCmd::ParseFile(const std::string& filename) {
	readRawData.clear();

	std::ifstream file(filename);
	CheckInputAndCreateInputFile(file, filename);
	FillReadRawAllDatas(file);
	file.close();
}


void SsdReadCmd::CheckInputAndCreateInputFile(std::ifstream& file, const std::string& filename)
{
	if (!file) {
		CreateNewNandFile(filename);
		ReOpenNandFile(file, filename);
	}
}


void SsdReadCmd::CreateNewNandFile(const std::string& filename)
{
	std::ofstream newFile(filename);
	if (!newFile) {
		throw std::runtime_error("ssd_nand.txt create failed");
	}

	FillZeroDataToAllAddresses(newFile);
	std::cout << filename << " Create New file because there was no ssd_nand.txt" << std::endl;
}
void SsdReadCmd::FillZeroDataToAllAddresses(std::ofstream& newFile)
{
	// Init data to 00000000 for 0-99 address
	for (int i = 0; i < 100; ++i) {
		newFile << std::hex << std::nouppercase;
		newFile << i << ";00000000" << std::endl;
	}
	newFile.close();
}

void SsdReadCmd::ReOpenNandFile(std::ifstream& file, const std::string& filename)
{
	file.open(filename);
	if (!file) {
		throw std::runtime_error("error opening ssd_nand.txt file again for reading");
	}
}

void SsdReadCmd::FillReadRawAllDatas(std::ifstream& file)
{
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::string addrStr;
		std::string dataStr;

		if (false == SplitStringToAddressAndData(line, addrStr, dataStr)) continue;
		if (false == CheckParsingLineSuccessAndPushReadRawData(line, addrStr, dataStr)) continue;
	}
}

bool SsdReadCmd::SplitStringToAddressAndData(std::string& line, std::string& addrStr, std::string& dataStr)
{
	size_t delimiterPos = line.find(';');
	if (delimiterPos == std::string::npos) return false;

	addrStr = line.substr(0, delimiterPos);
	dataStr = line.substr(delimiterPos + 1);
	return true;
}

bool SsdReadCmd::CheckParsingLineSuccessAndPushReadRawData(const std::string& line, const std::string& addrStr, const std::string& dataStr)
{
	try {
		uint32_t  addr = std::stoul(addrStr, nullptr, 16);
		uint32_t  data = std::stoul(dataStr, nullptr, 16);

		readRawData.push_back({ addr, data });
	}
	catch (const std::exception& e) {
		std::cerr << "ÆÄ½Ì ½ÇÆÐ: " << e.what() << ", line=" << line << std::endl;
		return false;
	}
	return true;
}

void SsdReadCmd::readNandData(const std::string& filename) {
	if (!readRawData.empty()) readRawData.clear();

	ParseFile(filename);

	auto foundReadData = std::find_if(
		readRawData.begin(),
		readRawData.end(),
		[this](const ReadRawData& readEntry) {
			return readEntry.address == requestedAddress;
		}
	);

	if (foundReadData != readRawData.end()) {
		readData = foundReadData->data;
		std::ofstream outFile(OUTPUT_FILENAME);
		outFile << "0x" << std::hex << std::uppercase << readData << std::endl;
		return;
	}
	else {
		std::ofstream outFile(OUTPUT_FILENAME);
		outFile << OUTPUT_ERROR << std::endl;
		return;
	}
}