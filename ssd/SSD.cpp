#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "../ssd/INAND.cpp"
#include <stdexcept>

class SSD {
public:
	void selectNAND(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, std::string value) {
		try {
			checkLbaRange(lba);
			checkValue(value);
		}
		catch (std::exception e) {
			return;
		}

		nand_->write(lba, value);
	}
	std::string read(int lba) {	
		try {
			checkLbaRange(lba);
		}
		catch (std::exception e) {
			return "";
		}

		std::string value = nand_->read(lba);
		writeToResultFile(value);
		return value;
	}
	void erase(int lba, int size) {
		try {
			checkLbaRange(lba);
			checkLbaRange(lba + size - 1);
			checkEraseSize(size);
		}
		catch (std::exception e) {
			return;
		}

		nand_->erase(lba, size);
		std::cout << lba << " " << size << std::endl;
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