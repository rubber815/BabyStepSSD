#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "../SSDProject/INAND.cpp"
#include <stdexcept>

class SSD {
public:
	void selectNAND(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, std::string value) {
		checkLbaRange(lba);
		checkValue(value);

		nand_->write(lba, value);
	}
	std::string read(int lba) {	
		checkLbaRange(lba);

		std::string value = nand_->read(lba);
		writeToResultFile(value);
		return value;
	}

	// TODO
	bool verifyCommandFormat(const std::string& command) {
		std::istringstream iss(command);
		std::string op, lba, val;

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
			catch(std::exception e) {
				return false;
			}
			return true;
		}
		
		if(op == "fullread") {
			// TODO: detail format check
			return true;
		}

		if (op == "write") {
			iss >> lba >> val;
			// TODO: detail format check
			return true;
		}
		
		if (op == "read") {
			// TODO: detail format check
			iss >> lba;
			return true;
		}

		/*Test App format verification*/
		if (op == "testapp1") {
			// TODO: detail format check
			return true;
		}

		/*RW op formant not verified!!!*/
		return false;
	}
private:
	INAND * nand_;

	const int MIN_LBA = 0;
	const int MAX_LBA = 99;
	const int VALUE_LENGTH = 10;
	const std::string PREFIX_VALUE = "0x";
	const std::string RESULT_FILE_NAME = "result.txt";

	void checkLbaRange(int lba) {
		if (lba < MIN_LBA || lba > MAX_LBA)
			throw std::invalid_argument("lba is incorrect");
	}

	void checkValue(std::string value) {
		if (value.length() != VALUE_LENGTH
			|| (value.substr(0, 2) != PREFIX_VALUE)) {
			throw std::invalid_argument("value is incorrect");
		}

		for (int i = 2; i < value.length(); i++) {
			if (value[i] < '0' || value[i] > '9' 
				|| value[i] < 'A' || value[i] > 'F')
				throw std::invalid_argument("value is incorrect");
		}
	}

	void writeToResultFile(std::string value) {
		std::ofstream outputFile(RESULT_FILE_NAME);
		if (!outputFile) {
			std::cout << "Failed to open file for writing." << std::endl;
		}
		else {
			outputFile << value << std::endl;
			outputFile.close();
		}
	}
};