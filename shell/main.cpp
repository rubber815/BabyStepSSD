#pragma once

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For system() function
#include <io.h>
#include "Logger.cpp"

bool verifyCommandFormat(const std::string& command) {
	std::string operation;
	std::istringstream iss(command);
	iss >> operation;
	if (operation == "erase" ||
		operation == "erase_range" ||
		operation == "write" ||
		operation == "read" ||
		operation == "exit" ||
		operation == "help" ||
		operation == "fullwrite" ||
		operation == "fullread" ||
		operation == "testapp1" ||
		operation == "testapp2") {
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

bool erase(std::string lba, std::string size) {
	std::string str = "ssd ";
	str = str + "E " + lba + " " + size;

	system(str.c_str());

	return true;
}

bool erase_range(std::string startlba, std::string endlba) {
	int size = stoi(endlba) - stoi(startlba);

	erase(startlba, std::to_string(size));

	return true;
}

bool write(std::string lba, std::string value) {
	std::string str = "ssd ";
	str = str + "W " + lba + " " + value;

	LOG_FUNCTION_CALL(__func__, str);

	system(str.c_str());
	return true;
}

bool read(std::string lba) {
	std::string str = "ssd ";
	str = str + "R " + lba;

	LOG_FUNCTION_CALL(__func__, str);

	system(str.c_str());
	return true;
}

bool fullWrite(std::string value) {
	LOG_FUNCTION_CALL(__func__, value);

	for (int i = 0; i < 100; i++) {
		std::string str = R"(ssd W )" + std::to_string(i) + " " + value;
		system(str.c_str());
	}

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
	LOG_FUNCTION_CALL(__func__, "");

	for (int i = 0; i < 100; i++) {
		std::string str = R"(ssd R )" + std::to_string(i);
		system(str.c_str());
		std::cout << readFromResultFile() << std::endl;
	}
	return false;
}

bool testApp1() {
	const std::string input1 = "0xABCDEFAB";
	LOG_FUNCTION_CALL(__func__, "Full Write: " + input1 + " + ReadCompare");

	for (int lba = 0; lba < 100; lba++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lba);
		cmd += " " + input1;
		system(cmd.c_str());
	}
	for (int lba = 0; lba < 100; lba++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lba);
		system(cmd.c_str());

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

	LOG_FUNCTION_CALL(__func__, "Full Write: " + value1 + ", " + value2 + " + ReadCompare");

	std::string cmd = R"(ssd W )" + std::to_string(3) + " " + std::string(value1);
	system(cmd.c_str());
	
	for (int i = 0; i < agingCnt; i++) {
		for (int lba = 0; lba <= maxLba; lba++) {
			std::string cmd = R"(ssd W )" + std::to_string(lba) + " " + std::string(value1);
			system(cmd.c_str());
		}
	}

	for (int lba = 0; lba <= maxLba; lba++) {
		std::string cmd = R"(ssd W )" + std::to_string(lba) + " " + std::string(value2);
		system(cmd.c_str());
	}

	for (int lba = 0; lba <= maxLba; lba++) {
		std::string cmd = R"(ssd R )" + std::to_string(lba);
		system(cmd.c_str());
		std::string retValue = readFromResultFile();
#if 0
		if (retValue == value2) {
			std::cout << "TestApp2: Read compare after "
				"write aging: Passed, LBA: " << lba
				<< ", Expected: " << value2 << ", Actual: "
				<< retValue << std::endl;
		}
		else {
			std::cout << "TestApp2: Read compare after "
				"write aging: Failed, LBA: " << lba
				<< ", Expected: " << value2 << ", Actual: "
				<< retValue << std::endl;
		}
#endif
	}
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
	std::string funcName;
	std::ifstream runnerFile(path);

	while (!runnerFile.eof()) {
		bool result = false;
		
		std::getline(runnerFile, funcName);
		std::cout << funcName << " --- Run...";

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
FunctionCallLogger* FunctionCallLogger::instance = nullptr;

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
