#pragma once

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
		// TODO
	}
private:
	INAND * nand_;
};