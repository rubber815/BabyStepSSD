#pragma once

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For system() function
#include <io.h>
#include "Logger.cpp"

#define APP_NAME "ssd "

const int MIN_LBA = 0;
const int MAX_LBA = 99;
const int VALUE_LENGTH = 10;
const std::string PREFIX_VALUE = "0x";

void checkLbaRange(int lba) {
	if (lba < MIN_LBA || lba > MAX_LBA)
		throw std::invalid_argument("lba is incorrect");
}

void checkEraseSize(int size) {
	if (size > 10)
		throw std::invalid_argument("over size");
}

void checkValue(std::string value) {
	if (value.length() != VALUE_LENGTH)
		throw std::invalid_argument("VALUE has a maximum length of 10.");

	if (value.substr(0, 2) != PREFIX_VALUE)
		throw std::invalid_argument("VALUE must be entered as 0x in hexadecimal.");

	for (int i = 2; i < value.length(); i++) {
		if (!(value[i] >= '0' && value[i] <= '9')
			&& !(value[i] >= 'A' && value[i] <= 'F'))
			throw std::invalid_argument("VALUE can be entered from 0 to 9 or A to F.");
	}
}

bool verifyCommandFormat(const std::string& command) {
	std::istringstream iss(command);
	std::string op, lba, endlba, val, size;

	iss >> op;

	/*exit: terminate shell*/
	if (op == "exit") {
		return true;
	}

	/*help: print the usage for each command.*/
	if (op == "help") {
		return true;
	}

	/*RW op format verification*/
	if (op == "fullwrite") {
		iss >> val;
		try {
			checkValue(val);
		}
		catch (std::invalid_argument e) {
			std::cout << e.what() << std::endl;
			return false;
		}
		return true;
	}

	if (op == "fullread") {
		// TODO: detail format check
		return true;
	}

	if (op == "write") {
		iss >> lba >> val;

		try {
			checkLbaRange(stoi(lba));
			checkValue(val);
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

	if (op == "read") {
		iss >> lba;

		try {
			checkLbaRange(stoi(lba));
		}
		catch (std::exception e) {
			return false;
		}

		return true;
	}

	if (op == "erase") {
		iss >> lba;
		iss >> size;

		try {
			checkLbaRange(stoi(lba));
			checkLbaRange(stoi(lba) + stoi(size) - 1);
			checkEraseSize(stoi(size));
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

	if (op == "erase_range") {
		iss >> lba;
		iss >> endlba;

		try {
			checkLbaRange(stoi(lba));
			checkLbaRange(stoi(endlba) - 1);
			checkEraseSize(stoi(endlba) - stoi(lba));
		}
		catch (std::exception e) {
			return false;
		}
		return true;
	}

	/*Test App format verification*/
	if (op == "testapp1") {
		// TODO: detail format check
		return true;
	}

	if (op == "testapp2") {
		// TODO: detail format check
		return true;
	}

	std::cout << "INVALID COMMAND" << std::endl;
	return false;
}


void help() {
	std::cout << std::endl;
	std::cout << "************************************" << std::endl;
	std::cout << "* [ShellProgram commands help]" << std::endl;
	std::cout << "* exit: terminate shell" << std::endl;
	std::cout << "* help: print the usage for each command" << std::endl;
	std::cout << "* erase: erase LBA ~ (LBA + SIZE)" << std::endl;
	std::cout << "*		format: erase <LBA> <SIZE>" << std::endl;
	std::cout << "* erase_range: erase STARTLBA ~ ENDLBA" << std::endl;
	std::cout << "*		format: write <STARTLBA> <ENDLBA>" << std::endl;
	std::cout << "* write: write to SSD" << std::endl;
	std::cout << "*		format: write <LBA> <VALUE>" << std::endl;
	std::cout << "* read: read from SSD" << std::endl;
	std::cout << "*		format: read <LBA>" << std::endl;
	std::cout << "* fullwrite: write from LBA numbers 0 to 99." << std::endl;
	std::cout << "*		format: fullwrite <VALUE>" << std::endl;
	std::cout << "* fullread: read from LBA numbers 0 to 99." << std::endl;
	std::cout << "*		format: fullread" << std::endl;
	std::cout << "*----------------------------------" << std::endl;
	std::cout << "* LBA: 0 ~ 99                      " << std::endl;
	std::cout << "* VALUE: 0x00000000 ~ 0xFFFFFFFF   " << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << std::endl;
}
bool exit() {
	std::cout << "Exiting the program..." << std::endl;
	return false;
}

std::string makeSSDCommand(std::string cmd, std::string arg1, std::string arg2) {
	std::string result;
	result = APP_NAME + cmd + " " + arg1 + " " + arg2;

	return result;
}

bool erase(std::string lba, std::string size) {
	system(makeSSDCommand("E", lba, size).c_str());

	return true;
}

bool erase_range(std::string startlba, std::string endlba) {
	int size = stoi(endlba) - stoi(startlba);

	erase(startlba, std::to_string(size));

	return true;
}

bool write(std::string lba, std::string value) {
	std::string str = makeSSDCommand("W", lba, value).c_str();
	LOG_FUNCTION_CALL(str);
	system(str.c_str());

	return true;
}
bool read(std::string lba) {
	std::string str = makeSSDCommand("R", lba, "").c_str();
	LOG_FUNCTION_CALL(str);
	system(str.c_str());

	return true;
}

bool fullWrite(std::string value) {
	LOG_FUNCTION_CALL(value);
	for (int i = 0; i < 100; i++)
		system(makeSSDCommand("W", std::to_string(i), value).c_str());
	return false;
}

std::string readFromResultFile() {
	const std::string RESULT_FILE_NAME = "result.txt";
	std::ifstream inputFile(RESULT_FILE_NAME);
	std::string value;

	if (!inputFile.is_open()) {
		std::cout << "Failed to open file for reading." << std::endl;
		return value;
	}
	std::getline(inputFile, value);

	inputFile.close();
	return value;

}

bool fullRead() {
	LOG_FUNCTION_CALL("");

	for (int i = 0; i < 100; i++) {
		system(makeSSDCommand("R", std::to_string(i), "").c_str());
		std::cout << readFromResultFile() << std::endl;
	}
	return false;
}

bool testApp1() {
	const std::string input1 = "0xABCDEFAB";
	LOG_FUNCTION_CALL("Full Write: " + input1 + " + ReadCompare");

	for (int lba = 0; lba < 100; lba++)
		system(makeSSDCommand("W", std::to_string(lba), input1).c_str());

	for (int lba = 0; lba < 100; lba++) {
		system(makeSSDCommand("R", std::to_string(lba), "").c_str());

		// Compare
		std::string read_val = readFromResultFile();
		if (read_val != input1)
			return false;
	}

	return true;
}

bool testApp2() {
	const int agingCnt = 30;
	const int maxLba = 5;
	const std::string value1 = "0xAAAABBBB";
	const std::string value2 = "0x12345678";

	LOG_FUNCTION_CALL("Full Write: " + value1 + ", " + value2 + " + ReadCompare");

	for (int i = 0; i < agingCnt; i++) {
		for (int lba = 0; lba <= maxLba; lba++)
			system(makeSSDCommand("W", std::to_string(lba), value1).c_str());
	}

	for (int lba = 0; lba <= maxLba; lba++)
		system(makeSSDCommand("W", std::to_string(lba), value2).c_str());

	for (int lba = 0; lba <= maxLba; lba++)
		system(makeSSDCommand("R", std::to_string(lba), "").c_str());

	return true;
}

bool isValidFilePath(char* path) {
	std::ifstream runnerFile(path);

	if (!runnerFile.is_open()) {
		std::cout << "Failed to open file for reading." << std::endl;
		return false;
	}
	return true;
}

void doRunner(char* path) {
	bool result;
	std::string funcName;
	std::ifstream runnerFile(path);

	while (!runnerFile.eof()) {
		result = false;
		
		std::getline(runnerFile, funcName);
		if (funcName == "testApp1") {
			result = testApp1();
		}
		else if (funcName == "testApp2") {
			result = testApp2();
		}
		else {
			if (_access(funcName.c_str(), 0) != -1) {
				system(funcName.c_str());
				result = true;
			}
		}
		std::cout << funcName << " --- Run...";

		if (result) {
			std::cout << "Pass" << std::endl;
		}
		else {
			std::cout << "FAIL!" << std::endl;
			break;
		}
	}
}

// Static member initialization
Logger* Logger::instance = nullptr;

int main(int argc, char* argv[]) {
	// for Runner
	if (argc > 1) {
		if (!isValidFilePath(argv[1])) {
			return 0;
		}
		doRunner(argv[1]);
			
		return 0;
	}

	std::string command, operation, lba, endlba, value, size;
	std::string cmd = "ssd ";

	while (true) {
		std::cout << "Welcome to ShellProgram!!: "; // Updated prompt
		std::getline(std::cin, command);

		if (!(verifyCommandFormat(command)))
			continue;

		// parsing commands
		std::istringstream iss(command);
		iss >> operation;

		// Shell commands
		if (operation == "exit") {
			exit();
			break;
		}

		if (operation == "help") {
			help();
			continue;
		}

		/*RW commands*/
		if (operation == "erase") {
			iss >> lba;
			iss >> size;
			erase(lba, size);
		}
		else if (operation == "erase_range") {
			iss >> lba;
			iss >> endlba;
			erase_range(lba, endlba);
		}
		else if (operation == "write") {
			iss >> lba;
			iss >> value;
			write(lba, value);
		}
		else if (operation == "read") {
			iss >> lba;
			read(lba);
		}
		else if (operation == "fullwrite") {
			iss >> value;	
			fullWrite(value);
		}
		else if (operation == "fullread") {
			fullRead();
		}
		/*TestApp commands*/
		else if (operation == "testapp1") {
			if (testApp1())
				std::cout << "testapp1: Compare Success!" << std::endl;
			else
				std::cout << "testapp1: Compare Fail!" << std::endl;
		}
		else if (operation == "testapp2")
			testApp2();

		// for reference: Execute the .exe file
		//system("ssd W 1 0x11111111");
		//system("ssd W 2 0x22222222");
		//system("ssd R 1");
	}
}
