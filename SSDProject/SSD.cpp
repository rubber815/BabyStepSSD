#pragma once

#include <iostream>
#include <fstream>

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

		std::string str = nand_->read(lba);
		std::ofstream outputFile("result.txt");
		if (!outputFile) {
			std::cout << "Failed to open file for writing." << std::endl;
		}
		else {
			outputFile << str << std::endl;
			outputFile.close();
		}
		return str;
	}
private:
	INAND * nand_;

	const int MIN_LBA = 0;
	const int MAX_LBA = 99;
	const int VALUE_LENGTH = 10;
	const std::string PREFIX_VALUE = "0x";

	void checkLbaRange(int lba) {
		if (lba < MIN_LBA || lba > MAX_LBA)
			throw std::invalid_argument("lba is incorrect");
	}

	void checkValue(std::string value) {
		if (value.length() != VALUE_LENGTH
			|| (value.substr(0, 2) != PREFIX_VALUE)) {
			throw std::invalid_argument("value is incorrect");
		}
	}
};