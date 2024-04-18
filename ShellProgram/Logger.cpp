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
    std::ofstream logfile;
    const std::string filename = "latest.log";
    static FunctionCallLogger* instance;
    int count = 0;

    FunctionCallLogger() {
        // Open the log file
        openLogFile();
    }

    void openLogFile() {
        logfile.open(filename, std::ios::app);
        if (!logfile.is_open()) {
            std::cerr << "Error: Unable to open log file\n";
        }
    }

    void closeLogFile() {
        if (logfile.is_open()) {
            logfile.close();
        }
    }

    void changeFileName(const std::tm& timeinfo) {
        std::ostringstream newFilename;
        newFilename << "until_" << std::put_time(&timeinfo, "%y%m%d_%H%M%S") << ".log";
        std::cout << newFilename.str() << std::endl;
        closeLogFile();
        std::rename(filename.c_str(), newFilename.str().c_str());
        count++;
        //filename = newFilename.str();
        if (count == 2) {
            std::cout << "two" << std::endl;
            std::ostringstream newZip;
            newZip << "until_" << std::put_time(&timeinfo, "%y%m%d_%H%M%S") << ".zip";
            std::rename(newFilename.str().c_str(), newZip.str().c_str());
        }
        openLogFile();
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
        if (logfile.is_open()) {
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
            std::cout << buffer << " " << functionName << " : " << message << std::endl;
            logfile << buffer << " " << functionName << " : " << message << std::endl;

            // Check logfile size and change filename if necessary
            logfile.flush();
            //if (logfile.tellp() > 10240) { // 10KB
            if (logfile.tellp() > 1) {
                changeFileName(timeinfo);
            }

            /*  1. When "two" until_date_time.log files are created,
                    Compresses the oldest created log file.
                2. Assuming you have used Compressed Library,
                    change only the file name from .log --> .zip.*/
            if (count == 2) {
                std::cout << "two" << std::endl;
            }
        }
    }
};

// Static member initialization
FunctionCallLogger* FunctionCallLogger::instance = nullptr;

// Macro to simplify logging
#define LOG_FUNCTION_CALL(FunctionName, Message) \
    FunctionCallLogger::getInstance()->print(FunctionName, Message)

// Example usage
void someFunction() {
    //LOG_FUNCTION_CALL(__FUNCTION__);
    // Your function logic here
    LOG_FUNCTION_CALL(__func__, "Some message");
    // Your function logic here
}