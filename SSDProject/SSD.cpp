#pragma once

#include "../SSDProject/INAND.cpp"

class SSD {
public:
	void selectStockBrocker(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, int value) {
		// TODO
	}
	int read(int lba) {
		// TODO
	}
private:
	INAND * nand_;
};