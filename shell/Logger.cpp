#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

#include <chrono>
#include <thread>

/*
Function call logger
that manages the log output by the test shell
*/
class Logger {
private:
	const std::string LOGGER_TIME_FORMAT = "[%y.%m.%d %H:%M:%S]";
	const std::string LATEST_LOG_FILENAME = "latest.log"; // default file name: will be changed to date_time.log

	std::ofstream latestLogFile;

	static Logger* instance;

	std::ostringstream logFile;
	std::ostringstream compessionCandidateFile;

	Logger() {
		openNewLogFile();
	}

	/* 1. ctor, 2. end of recordLog*/
	void openNewLogFile() {
		latestLogFile.open(LATEST_LOG_FILENAME, std::ios::app); // open new latest.log
		if (!latestLogFile.is_open()) {
			std::cerr << "Error: Unable to open new latest.log" << std::endl;
		}
	}

	void closeLogFile() {
		if (latestLogFile.is_open()) {
			latestLogFile.close(); // close latest.log
		}
		else {
			std::cerr << "Error: Unable to open latest.log" << std::endl;
		}
	}

	// Logger feature 2: Log recording rules
	/*from latest.log to until_date_time.log*/
	void recordLog(const std::tm& timeinfo) {
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
	void compressLog() {
		std::rename(logFile.str().c_str(), compessionCandidateFile.str().c_str());
		compessionCandidateFile.clear();
		compessionCandidateFile.str("");
	}

public:
	// Singleton instance
	static Logger* getInstance() {
		if (!instance) {
			instance = new Logger();
		}
		return instance;
	}

	~Logger() {
		closeLogFile();
	}

	/* Output format: [date time] function()         : log message */
	void print(const std::string& functionName, const std::string& message) {
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
};

// Static member initialization
//FunctionCallLogger* FunctionCallLogger::instance = nullptr;

// Macro to simplify logging
#define LOG_FUNCTION_CALL(Message) \
	Logger::getInstance()->print(__FUNCTION__, Message)