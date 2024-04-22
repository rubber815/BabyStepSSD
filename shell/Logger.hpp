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

	Logger();

	/* 1. ctor, 2. end of recordLog*/
	void openNewLogFile();

	void closeLogFile();

	// Logger feature 2: Log recording rules
	/*from latest.log to until_date_time.log*/
	void recordLog(const std::tm& timeinfo);

	// Logger feature 2: Log compression rules
	/*from .log to .zip */
	void compressLog();
public:
	// Singleton instance
	static Logger* getInstance();

	~Logger();

	/* Output format: [date time] function()         : log message */
	void print(const std::string& functionName, const std::string& message);

};

// Static member initialization
//FunctionCallLogger* FunctionCallLogger::instance = nullptr;

// Macro to simplify logging
#define LOG_FUNCTION_CALL(Message) \
	Logger::getInstance()->print(__FUNCTION__, Message)