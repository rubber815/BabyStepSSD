#pragma once

#include "INAND.cpp"
#include <fstream>
#include <vector>

#include "BabyStepNAND.hpp"

BabyStepNand::BabyStepNand() {
	if (readAllNand() == false) {
		for (int i = 0; i < NAND_LBA_COUNT; i++)
			m_buffer.push_back(NAND_DEFAULT_VALUE);

		writeAllNand();
	}
}

void BabyStepNand::erase(int lba, int size) {
	readAllNand();

	for (int i = 0; i < size; i++) {
		m_buffer[lba + i] = NAND_DEFAULT_VALUE;
	}

	writeAllNand();
}

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

bool BabyStepNand::readAllNand() {
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

void BabyStepNand::writeAllNand() {
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