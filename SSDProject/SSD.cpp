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
		if (lba < 0 || lba > 100)
			throw std::invalid_argument("lba is incorrect");

		if (value.length() != 10
			|| (value.substr(0, 2) != "0x")) {
			throw std::invalid_argument("value is incorrect");
		}
		nand_->write(lba, value);
	}
	std::string read(int lba) {
		std::string str;
		if (lba < 0 || lba > 100) {
			std::cout << "Exceeds LBA scope!" << std::endl;
			str = "0x00000000";
		}
		else {
			str = nand_->read(lba);
		}

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
};