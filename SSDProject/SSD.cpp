#pragma once

#include <iostream>

#include "../SSDProject/INAND.cpp"

class SSD {
public:
	void selectNAND(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, std::string value) {
		// TODO
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
		return str;
	}
private:
	INAND * nand_;
};