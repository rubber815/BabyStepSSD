#pragma once

#include "INAND.cpp"
#include <fstream>
#include <vector>

#define NAND_FILE "nand.txt"
#define NAND_DEFAULT_VALUE "0x00000000"
#define NAND_LBA_COUNT 100

class BabyStepNand : public INAND {
public:
	BabyStepNand();

	void erase(int lba, int size) override;

	void write(int lba, std::string value) override;

	std::string read(int lba) override;

private:
	std::vector<std::string> m_buffer;

	bool readAllNand();

	void writeAllNand();
};