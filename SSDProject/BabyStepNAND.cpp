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
	std::vector<std::string> m_buffer;

	void readAllNand() {
		std::ifstream readFromNand;
		readFromNand.open("nand.txt");

		if (readFromNand.is_open()) {
			m_buffer.clear();

			while (!readFromNand.eof()) {
				std::string tmp;
				getline(readFromNand, tmp);
				m_buffer.push_back(tmp);
			}
		}

		readFromNand.close();
	}

	void writeAllNand() {
		std::ofstream writeToNand;
		writeToNand.open("nand.txt");

		int len = static_cast<int>(m_buffer.size());
		for (int i = 0; i < len; i++) {
			std::string tmp = m_buffer[i];
			if (i != len - 1) {
				tmp += "\n";
			}
			writeToNand.write(tmp.c_str(), tmp.size());
		}

		writeToNand.close();
	}
};

void BabyStepNand::write(int lba, std::string value) {
	readAllNand();

	// Modify NAND data
	m_buffer[lba] = value;

	writeAllNand();
}

std::string BabyStepNand::read(int lba) {
	readAllNand();

	// Read NAND data
	std::string data{ m_buffer[lba] };

	return data;
}