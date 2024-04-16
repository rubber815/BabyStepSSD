#pragma once

#include "INAND.cpp"
#include <fstream>
#include <vector>

class BabyStepNand : public INAND {
public:
	BabyStepNand() {
		int len = 100;

		std::ofstream writeToNand;
		writeToNand.open("nand.txt");

		for (int i = 0; i < len; i++) {
			std::string tmp = "0x00000000";
			if (i != len - 1) {
				tmp += "\n";
			}
			writeToNand.write(tmp.c_str(), tmp.size());
		}
		writeToNand.close();
	}

	void write(int lba, std::string value) override;
	std::string read(int lba) override;

private:
	std::vector<std::string> buffer;

	void readAllNand() {
		std::ifstream readFromNand;
		readFromNand.open("nand.txt");

		if (readFromNand.is_open()) {
			this->buffer.clear();

			while (!readFromNand.eof()) {
				std::string tmp;
				getline(readFromNand, tmp);
				this->buffer.push_back(tmp);
			}
		}

		readFromNand.close();
	}
};

void BabyStepNand::write(int lba, std::string value) {
	readAllNand();

	// Modify NAND data
	buffer[lba] = value;


	// Write All NAND
	int len = static_cast<int>(buffer.size());

	std::ofstream writeToNand;
	writeToNand.open("nand.txt");

	for (int i = 0; i < len; i++) {
		std::string tmp = buffer[i];
		if (i != len - 1) {
			tmp += "\n";
		}
		writeToNand.write(tmp.c_str(), tmp.size());
	}
	writeToNand.close();
}

std::string BabyStepNand::read(int lba) {
	readAllNand();

	// Read NAND data
	std::string data{ buffer[lba] };

	return data;
}