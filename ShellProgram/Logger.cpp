#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

class FunctionCallLogger {
private:
    std::ofstream logfile;
    static FunctionCallLogger* instance;

    FunctionCallLogger() {
        // Open the log file
        logfile.open("function_calls.log");
        if (!logfile.is_open()) {
            std::cerr << "Error: Unable to open log file\n";
        }
    }

public:
    static FunctionCallLogger* getInstance() {
        if (!instance) {
            instance = new FunctionCallLogger();
        }
        return instance;
    }

    ~FunctionCallLogger() {
        // Close the log file
        if (logfile.is_open()) {
            logfile.close();
        }
    }

    void log(const std::string& functionName) {
        if (logfile.is_open()) {
            // Get current time
            std::time_t now = std::time(nullptr);
            std::tm timeinfo;
            localtime_s(&timeinfo, &now); // Use localtime_s instead of localtime

            // Format time
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "[%y.%m.%d %H:%M]", &timeinfo);

            // Log function call with time information
            logfile << buffer << " Function called: " << functionName << std::endl;
        }
    }
};

// Static member initialization
FunctionCallLogger* FunctionCallLogger::instance = nullptr;

// Macro to simplify logging
#define LOG_FUNCTION_CALL(FunctionName) \
    FunctionCallLogger::getInstance()->log(FunctionName)

// Example usage
void someFunction() {
    LOG_FUNCTION_CALL(__func__);
    // Your function logic here
}