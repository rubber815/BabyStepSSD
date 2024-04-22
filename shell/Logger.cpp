#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

#include <chrono>
#include <thread>

#include "Logger.hpp"
/*
Function call logger
that manages the log output by the test shell
*/

Logger::Logger() {
	openNewLogFile();
}

/* 1. ctor, 2. end of recordLog*/
void Logger::openNewLogFile() {
	latestLogFile.open(LATEST_LOG_FILENAME, std::ios::app); // open new latest.log
	if (!latestLogFile.is_open()) {
		std::cerr << "Error: Unable to open new latest.log" << std::endl;

		static Logger* instance;

		std::ostringstream logFile;
		std::ostringstream compessionCandidateFile;

		bool screenMode = true;
	}
}

void Logger::closeLogFile() {
	if (latestLogFile.is_open()) {
		latestLogFile.close(); // close latest.log
	}
	else {
		std::cerr << "Error: Unable to open latest.log" << std::endl;
	}
}

// Logger feature 2: Log recording rules
/*from latest.log to until_date_time.log*/
void Logger::recordLog(const std::tm& timeinfo) {
	if (compessionCandidateFile.str() != "") {
		compressLog();
	}
	logFile.clear();
	logFile.str("");
	//std::cout << "[Logger]: " << logFile.str() << " is created!" << std::endl;
	logFile << "until_" << std::put_time(&timeinfo, "%y%m%d_%Hh_%Mm_%Ss") << ".log";
	compessionCandidateFile << "until_" << std::put_time(&timeinfo, "%y%m%d_%Hh_%Mm_%Ss") << ".zip";
	//std::cout << "[Logger]: " << recordFile.str() << " is created!" << std::endl; // TODO : will be removed
	closeLogFile();

	std::rename(LATEST_LOG_FILENAME.c_str(), logFile.str().c_str()); // date log created!!

	openNewLogFile();
}

// Logger feature 2: Log compression rules
/*from .log to .zip */
void Logger::compressLog() {
	std::rename(logFile.str().c_str(), compessionCandidateFile.str().c_str());
	compessionCandidateFile.clear();
	compessionCandidateFile.str("");
}

// Singleton instance
Logger* Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
	}
	return instance;
}

Logger::~Logger() {
	closeLogFile();
}

/* Output format: [date time] function()         : log message */
void Logger::print(const std::string& functionName, const std::string& message) {
	if (latestLogFile.is_open()) {

		// Get current time
		std::time_t now = std::time(nullptr);
		std::tm currentTimeInfo;
		localtime_s(&currentTimeInfo, &now);

		// Format time [%y.%m.%d %H:%M:%S]
		char timeInfoPrefix[20];
		std::strftime(timeInfoPrefix, sizeof(timeInfoPrefix), LOGGER_TIME_FORMAT.c_str(), &currentTimeInfo);

		// Log function call with time information
		/* When the screen is output, the print function writes the file simultaneously.*/
		std::cout << timeInfoPrefix << " " << functionName << "() : " << message << std::endl; // screen print
		latestLogFile << timeInfoPrefix << " " << functionName << "() : " << message << std::endl; // latest.log file write

		// Check logfile size and change filename if necessary
		latestLogFile.flush();

		//std::cout << "Size: " << latestLogFile.tellp() << std::endl;
		// 10 KB = 10240 Bytes (in binary)
		if (latestLogFile.tellp() > 10240) {
			recordLog(currentTimeInfo);
		}
	}
}

void Logger::setScreenMode(bool mode) {
	screenMode = mode;
}