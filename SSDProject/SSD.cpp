#pragma once

#include "../SSDProject/INAND.cpp"
#include <stdexcept>

class SSD {
public:
	void selectNAND(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, std::string value) {
		// TODO
		if (lba < 0 || lba > 100)
			throw std::invalid_argument("lba is incorrect");
		nand_->write(lba, value);
	}
	std::string read(int lba) {
		// TODO
	}
private:
	INAND * nand_;
};