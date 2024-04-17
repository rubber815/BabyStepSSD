#pragma once

#include "INAND.cpp"
#include <fstream>
#include <vector>

#define NAND_FILE "nand.txt"
#define NAND_DEFAULT_VALUE "0x00000000"
#define NAND_LBA_COUNT 100

class BabyStepNand : public INAND {
public:
	BabyStepNand() {
		if (readAllNand() == false) {
			for (int i = 0; i < NAND_LBA_COUNT; i++)
				m_buffer.push_back(NAND_DEFAULT_VALUE);

			writeAllNand();
		}
	}

	void write(int lba, std::string value) override {
		readAllNand();

		// Modify NAND data
		m_buffer[lba] = value;

		writeAllNand();
	}

	std::string read(int lba) override {
		readAllNand();

		// Read NAND data
		std::string data{ m_buffer[lba] };

		return data;
	}

private:
	std::vector<std::string> m_buffer;

	bool readAllNand() {
		std::ifstream readFromNand;
		readFromNand.open(NAND_FILE);

		if (readFromNand.is_open()) {
			m_buffer.clear();

			while (!readFromNand.eof()) {
				std::string tmp;
				getline(readFromNand, tmp);
				m_buffer.push_back(tmp);
			}
		}
		else
		{
			return false;
		}

		readFromNand.close();
		return true;
	}

	void writeAllNand() {
		std::ofstream writeToNand;
		writeToNand.open(NAND_FILE);

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