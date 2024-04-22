#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include "INAND.cpp"
#include "SSD.hpp"

void SSD::selectNAND(INAND* nand) {
	nand_ = nand;
}

void SSD::checkLbaRange(int lba) {
	if (lba < MIN_LBA || lba > MAX_LBA)
		throw std::invalid_argument(ERR::INVALID_LBA);
}

void SSD::checkEraseSize(int size) {
	if (size > 10)
		throw std::invalid_argument(ERR::OVER_ERASE_SIZE);
}

void SSD::checkValue(std::string value) {
	if (value.length() != VALUE_LENGTH)
		throw std::invalid_argument(ERR::INVALID_VALUE_LENGTH);

	if (value.substr(0, 2) != PREFIX_VALUE)
		throw std::invalid_argument(ERR::INVALID_VALUE_PREFIX);

	for (int i = 2; i < value.length(); i++) {
		if (!(value[i] >= '0' && value[i] <= '9')
			&& !(value[i] >= 'A' && value[i] <= 'F'))
			throw std::invalid_argument(ERR::INVALID_VALUE_CHARACTER);
	}
}

void SSD::writeToResultFile(std::string value) {
	std::ofstream outputFile(RESULT_FILE_NAME);
	if (!outputFile) {
		std::cout << ERR::OPEN_WRITING_FILE_FAIL << std::endl;
	}
	else {
		outputFile << value << std::endl;
		outputFile.close();
	}
}

void SSD::write(int lba, std::string value) {
	try {
		checkLbaRange(lba);
		checkValue(value);
	}
	catch (std::exception e) {
		return;
	}

	nand_->write(lba, value);
}

std::string SSD::read(int lba) {
	try {
		checkLbaRange(lba);
	}
	catch (std::exception e) {
		return "";
	}

	std::string value = nand_->read(lba);
	writeToResultFile(value);
	return value;
}

void SSD::erase(int lba, int size) {
	try {
		checkLbaRange(lba);
		checkLbaRange(lba + size - 1);
		checkEraseSize(size);
	}
	catch (std::exception e) {
		return;
	}

	nand_->erase(lba, size);
	std::cout << lba << " " << size << std::endl;
}

bool SSD::updateWriteBuffer(char* argv[]) {
	int count = 0;
	std::string cmd = argv[1];
	if (cmd == "R") {
		return ReadWriteBuffer(argv);
	}
	else if (cmd == "F") {
		while (flushWriteBuffer() == false);
		return true;
	}
	else {
		count = AddCmdWriteBuffer(argv);
		if (count == -1) return false;
		if (count == 10) {
			while (flushWriteBuffer() == false);
		}
	}
	return true;
}

bool SSD::flushWriteBuffer(void) {
	int count;
	std::ifstream readFromWriteBuffer;
	readFromWriteBuffer.open(WRITE_BUFFER_FILE_NAME);

	if (readFromWriteBuffer.is_open()) {
		readFromWriteBuffer >> count;

		for (int i = 0; i < count; i++) {
			int old_lba;
			std::string old_cmd;
			std::string old_arg3;
			readFromWriteBuffer >> old_cmd;
			readFromWriteBuffer >> old_lba;
			readFromWriteBuffer >> old_arg3;

			if (old_cmd == "W") {
				write(old_lba, old_arg3);
			}
			else if (old_cmd == "E") {
				erase(old_lba, stoi(old_arg3));
			}
		}
		readFromWriteBuffer.close();
	}
	else {
		std::cout << "Failed to open file for reading." << std::endl;
		return false;
	}

	std::ofstream outputFile(WRITE_BUFFER_FILE_NAME);
	if (!outputFile) {
		std::cout << "Failed to open file for writing." << std::endl;
		return false;
	}
	else {
		outputFile << "0" << std::endl;
		outputFile.close();
	}

	return true;
}

bool SSD::ReadWriteBuffer(char* argv[]) {

	std::string str_argv2(argv[2]);
	std::string return_data = "FAIL";

	int count;
	std::ifstream readFromWriteBuffer;
	readFromWriteBuffer.open(WRITE_BUFFER_FILE_NAME);

	if (readFromWriteBuffer.is_open()) {
		readFromWriteBuffer >> count;

		for (int i = 0; i < count; i++) {
			int old_lba;
			std::string old_cmd;
			std::string old_arg3;
			readFromWriteBuffer >> old_cmd;
			readFromWriteBuffer >> old_lba;
			readFromWriteBuffer >> old_arg3;

			if (old_cmd == "W") {
				if (old_lba == stoi(str_argv2)) {
					return_data = old_arg3;
				}
			}
			else if (old_cmd == "E") {
				if ((old_lba <= stoi(str_argv2)) && ((old_lba + stoi(old_arg3)) > stoi(str_argv2))) {
					return_data = "0x00000000";
				}
			}
		}
		readFromWriteBuffer.close();

		if (return_data != "FAIL") {
			writeToResultFile(return_data);
		}
		else
			return false;
	}
	else {
		std::cout << "Failed to open file for reading." << std::endl;
		return false;
	}

	return true;
}

int SSD::AddCmdWriteBuffer(char* argv[]) {
	std::vector<std::string> v;
	std::string str_argv1(argv[1]);
	std::string str_argv2(argv[2]);
	std::string str_argv3(argv[3]);
	std::string tmp, input_str;
	int count;

	std::ifstream readFromWriteBuffer;
	readFromWriteBuffer.open(WRITE_BUFFER_FILE_NAME);

	if (readFromWriteBuffer.is_open()) {
		readFromWriteBuffer >> count;

		for (int i = 0; i < count; i++) {
			int old_lba;
			std::string old_cmd;
			std::string old_arg3;
			readFromWriteBuffer >> old_cmd;
			readFromWriteBuffer >> old_lba;
			readFromWriteBuffer >> old_arg3;
			tmp = old_cmd + " " + std::to_string(old_lba) + " " + old_arg3;

			if (old_cmd == "W") {
				// new command : write
				if (str_argv1 == "W") {
					if (old_lba == stoi(str_argv2)) {
						// same lba
					}
					else {
						v.push_back(tmp);
					}
				}
				// new command : erase
				else if (str_argv1 == "E") {
					if ((stoi(str_argv2) <= old_lba) && ((stoi(str_argv2) + stoi(str_argv3)) > old_lba)) {
						// same lba (range)
					}
					else {
						v.push_back(tmp);
					}
				}
			}
			else if (old_cmd == "E") {
				// new command : write
				if (str_argv1 == "W") {
					// erase after write : nothing to do
				}
				// new command : erase
				else if (str_argv1 == "E") {
					int old_size = stoi(old_arg3);
					int new_lba = stoi(str_argv2);
					int new_size = stoi(str_argv3);

					if (old_lba > new_lba) {
						if ((new_lba + new_size) >= old_lba) {
							if ((new_lba + new_size) >= (old_lba + old_size)) {
								// nothing to do
							}
							else {
								if (((old_lba + old_size) - new_lba) <= 10) {
									str_argv3 = std::to_string((old_lba + old_size) - new_lba);
								}
								else {
									v.push_back(tmp);
								}
							}
						}
						else {
							v.push_back(tmp);
						}
					}
					else {
						if ((old_lba + old_size) >= new_lba) {
							if ((old_lba + old_size) >= (new_lba + new_size)) {
								str_argv2 = std::to_string(old_lba);
								str_argv3 = std::to_string(old_size);
							}
							else {
								if (((new_lba + new_size) - old_lba) <= 10) {
									str_argv2 = std::to_string(old_lba);
									str_argv3 = std::to_string((new_lba + new_size) - old_lba);
								}
								else {
									v.push_back(tmp);
								}
							}
						}
						else {
							v.push_back(tmp);
						}
					}
				}
			}
		}

		input_str = str_argv1 + " " + str_argv2 + " " + str_argv3;

		v.push_back(input_str);

		readFromWriteBuffer.close();
	}
	else {
		std::cout << "Failed to open file for reading." << std::endl;
		return -1;
	}

	count = (int)(v.size());

	std::ofstream outputFile(WRITE_BUFFER_FILE_NAME);
	if (!outputFile) {
		std::cout << "Failed to open file for writing." << std::endl;
		return -1;
	}
	else {
		outputFile << count << std::endl;
		std::cout << count << std::endl;
		for (int i = 0; i < count; i++) {
			outputFile << v[i] << std::endl;
			std::cout << v[i] << std::endl;
		}
		outputFile.close();
	}

	return count;
}

ReadCommand::ReadCommand(SSD* ssd, int address)
	: ssd(ssd), address(address) {}

void ReadCommand::execute() {
	result = ssd->read(address);
}

std::string ReadCommand::getResult() {
	return result;
}

WriteCommand::WriteCommand(SSD* ssd, int address, const std::string& data)
	: ssd(ssd), address(address), data(data) {}

void WriteCommand::execute() {
	ssd->write(address, data);
}

EraseCommand::EraseCommand(SSD* ssd, int startAddress, int endAddress)
	: ssd(ssd), startAddress(startAddress), endAddress(endAddress) {}

void EraseCommand::execute() {
	ssd->erase(startAddress, endAddress);
}

void SSDInvoker::setCommand(Command* cmd) {
	command = cmd;
}

void SSDInvoker::executeCommand() {
	if (command)
		command->execute();
	else
		std::cout << ERR::NO_COMMAND_SET;
}

std::string SSDInvoker::getResult() {
	return command->getResult();
}