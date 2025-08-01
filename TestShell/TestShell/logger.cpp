#include "logger.h"

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <filesystem>

using std::string;

std::string getClassName(const std::string& funcSig) {
	size_t colons = funcSig.find("::");
	if (colons == std::string::npos) return "UnknownClass";

	size_t start = funcSig.rfind(' ', colons);
	if (start == std::string::npos) start = 0;
	else start += 1;

	return funcSig.substr(start, colons - start);
}

std::string getMethodName(const std::string& funcSig) {
	size_t colons = funcSig.find("::");
	if (colons == std::string::npos) return "UnknownMethod";
	return funcSig.substr(colons + 2);
}

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

bool Logger::initLogFile() {
	try {
		std::filesystem::create_directories(logDir);
		std::ofstream file(LOGFILE, std::ios::trunc);
		if (!file.is_open()) {
			throw std::ios_base::failure("Failed to open log file: " + LOGFILE);
		}

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to initialize log.txt.\n";
		std::cerr << e.what() << "\n";

		return false;
	}
}

void Logger::addLog(const string& prettyName, const string& msg) {

	string className = getClassName(prettyName).append(".");
	string methodName = getMethodName(prettyName);
	className.append(methodName);

	LogEntry entry = makeLogEntry(className, msg);

	rotateLogFileIfNeeded();

	writeLogToFile(entry);
}

void Logger::rotateLogFileIfNeeded()
{
	if (!isFileSizeOverTenKb(LOGFILE))
		return;

	string newFilename = getTimestampedFileName();
	renameFile(LOGFILE, getTimestampedFileName());

	if (isExistOldLogFile())
		compressOldLogFile(getOldLogFilename());

	setOldLogFilename(newFilename);
}

LogEntry Logger::makeLogEntry(const string& funcName, const string& msg)
{
	std::time_t t = std::time(nullptr);
	std::tm tm{};
	localtime_s(&tm, &t);

	LogEntry entry;
	entry.timestamp = tm;
	std::snprintf(entry.functionName, sizeof(entry.functionName), "%s( )", funcName.c_str());
	entry.message = msg.substr(0, 512);
	logs.push_back(entry);

	return entry;
}

void Logger::printSingleLog(const LogEntry& log)
{
	if (isRunnerMode == true) return;
	std::cout << formatLogLine(log) << std::endl;
}

void Logger::writeLogToFile(const LogEntry& log)
{
	std::ofstream file(LOGFILE, std::ios::app);
	if (!file.is_open()) {
		std::cerr << "Failed to open log.txt for writing.\n";
		return;
	}

	file << formatLogLine(log) << "\n";
}

string Logger::formatLogLine(const LogEntry& log) {
	std::ostringstream oss;

	oss << "[" << std::setfill('0')
		<< std::right << std::setw(2) << (log.timestamp.tm_year % 100) << "."
		<< std::setw(2) << (log.timestamp.tm_mon + 1) << "."
		<< std::setw(2) << log.timestamp.tm_mday << " "
		<< std::setw(2) << log.timestamp.tm_hour << ":"
		<< std::setw(2) << log.timestamp.tm_min << "] ";

	oss << std::setfill(' ') << std::left << std::setw(60) << log.functionName;

	string msg = log.message;
	std::replace(msg.begin(), msg.end(), '\n', ' ');
	oss << ": " << msg;

	return oss.str();
}

string Logger::getTimestampedFileName() {
	std::filesystem::create_directories(logDir);
	std::time_t now = std::time(nullptr);
	std::tm timeInfo;

	localtime_s(&timeInfo, &now);

	std::ostringstream oss;
	oss << "until_"
		<< std::setw(2) << std::setfill('0') << (timeInfo.tm_year % 100)   // YY
		<< std::setw(2) << std::setfill('0') << (timeInfo.tm_mon + 1)      // MM
		<< std::setw(2) << std::setfill('0') << timeInfo.tm_mday           // DD
		<< "_"
		<< std::setw(2) << std::setfill('0') << timeInfo.tm_hour << "h_"
		<< std::setw(2) << std::setfill('0') << timeInfo.tm_min << "m_"
		<< std::setw(2) << std::setfill('0') << timeInfo.tm_sec << "s"
		<< ".log";

	return logDir + "/" + oss.str();
}

bool Logger::isFileSizeOverTenKb(const string& filePath) {
	struct stat fileStat;
	if (stat(filePath.c_str(), &fileStat) != 0) {
		return false;
	}
	return fileStat.st_size >= MAX_LOG_SIZE;
}

void Logger::renameFile(const string& oldName, const string& newName) {
	if (std::rename(oldName.c_str(), newName.c_str()) != 0) {
		std::perror(("Failed to rename " + oldName + " to " + newName).c_str());
	}
	else {
		addLog(__FUNCSIG__, "Renamed " + oldName + "��" + newName);
	}
}

void Logger::setRunnerMode(bool runnerMode)
{
	isRunnerMode = runnerMode;
}

void Logger::compressOldLogFile(string lastLogFile) {
	try {
		std::filesystem::path oldLogPath(lastLogFile);
		std::filesystem::path zipName = oldLogPath;
		zipName.replace_extension(".zip");

		if (!std::filesystem::exists(oldLogPath)) {
			std::cerr << "Old log file does not exist: " << oldLogPath << std::endl;
			return;
		}

		std::filesystem::rename(oldLogPath, zipName);
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Failed to compress log file: " << e.what() << std::endl;
		std::abort();
	}
}

bool Logger::isExistOldLogFile()
{
	if (oldLogFilename == "")
		return false;
	else
		return true;
}

void Logger::setOldLogFilename(string filename)
{
	oldLogFilename = filename;
}

string Logger::getOldLogFilename(void) {
	return oldLogFilename;
}
