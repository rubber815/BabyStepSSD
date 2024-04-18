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
class FunctionCallLogger {
private:
    const std::string LATEST_LOG_FILENAME = "latest.log"; // default file name: will be changed to date_time.log

    std::ofstream latestLogFile;

    static FunctionCallLogger* instance;
    int count = 0; // 

    std::string theOldestFileName = "INVALID_FILE_NAME";

    FunctionCallLogger() {
        openLogFile();
    }

    /* 1. ctor, 2. end of changeFileName*/
    void openLogFile() {
        latestLogFile.open(LATEST_LOG_FILENAME, std::ios::app); // open new latest.log
        if (!latestLogFile.is_open()) {
            std::cerr << "Error: Unable to open new latest.log" << std::endl;
        }
    }

    void closeLogFile() {
        if (latestLogFile.is_open()) {
            latestLogFile.close(); // close latest.log
        }
    }

    void changeFileName(const std::tm& timeinfo) {
        // TODO:
    }
public:
    // Singleton instance
    static FunctionCallLogger* getInstance() {
        if (!instance) {
            instance = new FunctionCallLogger();
        }
        return instance;
    }

    ~FunctionCallLogger() {
        closeLogFile();
    }

    /* Output format: [date time] function name()         : log message */
    void print(const std::string& functionName, const std::string& message) {
        if (latestLogFile.is_open()) {

            // Get current time
            std::time_t now = std::time(nullptr);
            std::tm timeinfo;
            localtime_s(&timeinfo, &now); // Use localtime_s instead of localtime

            // Format time
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "[%y.%m.%d %H:%M:%S]", &timeinfo);

            // Log function call with time information
            /* When the screen is output,
            the print function writes the file simultaneously.*/
            std::cout << buffer << " " << functionName << " : " << message << std::endl; // screen print
            latestLogFile << buffer << " " << functionName << " : " << message << std::endl; // latest.log file write

            // TODO: Check logfile size and change filename if necessary
            //latestLogFile.flush();
            ////if (logfile.tellp() > 10240) { // 10KB
            //if (latestLogFile.tellp() > 1) {
            //    changeFileName(timeinfo);
            //}

            // TODO
            /*  1. When "two" until_date_time.log files are created,
                    Compresses the oldest created log file.
                2. Assuming you have used Compressed Library,
                    change only the file name from .log --> .zip.*/
        }
    }
};

// Static member initialization
//FunctionCallLogger* FunctionCallLogger::instance = nullptr;

// Macro to simplify logging
#define LOG_FUNCTION_CALL(FunctionName, Message) \
    FunctionCallLogger::getInstance()->print(FunctionName, Message)

// Example usage
// TODO: will be deleted
//void someFunction() {
//    //LOG_FUNCTION_CALL(__FUNCTION__);
//    // Your function logic here
//    LOG_FUNCTION_CALL(__func__, "Some message");
//    // Your function logic here
//}