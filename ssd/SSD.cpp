#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include "INAND.cpp"
#include "SSD.hpp"

SSD::SSD() {
	std::ifstream readFromWriteBuffer;
	readFromWriteBuffer.open(WRITE_BUFFER_FILE_NAME);

	if (readFromWriteBuffer.is_open() == false) {
		std::ofstream outputFile(WRITE_BUFFER_FILE_NAME);
		if (!outputFile) {
			std::cout << ERR::OPEN_WRITING_FILE_FAIL << std::endl;
		}
		else {
			outputFile << "0" << std::endl;
			outputFile.close();
		}
	}
}

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

	std::string value = readWriteBuffer(lba);
	if (value == "FAIL") {
		value = nand_->read(lba);
		writeToResultFile(value);
	}

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

bool SSD::updateWriteBuffer(std::string cmd, int lba, std::string argv3) {
	int count = 0;
	
	if ((cmd == "W") || (cmd == "E")) {
		count = AddCmdWriteBuffer(cmd, lba, argv3);
		if (count == -1) return false;
		if (count == 10) {
			while (flushWriteBuffer() == false);
		}

		return true;
	}
	return false;
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

std::string SSD::readWriteBuffer(int lba) {
	int count;
	std::string return_data = "FAIL";
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
				if (old_lba == lba) {
					return_data = old_arg3;
				}
			}
			else if (old_cmd == "E") {
				if ((old_lba <= lba) && ((old_lba + stoi(old_arg3)) > lba)) {
					return_data = "0x00000000";
				}
			}
		}
		readFromWriteBuffer.close();
	}
	else
		std::cout << "Failed to open file for reading." << std::endl;

	return return_data;
}

int SSD::AddCmdWriteBuffer(std::string cmd, int lba, std::string argv3) {
	std::vector<std::string> v;
	int new_lba = lba;
	std::string str_argv3 = argv3;
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
				if (cmd == "W") {
					if (old_lba == new_lba) {
						// same lba
					}
					else {
						v.push_back(tmp);
					}
				}
				// new command : erase
				else if (cmd == "E") {
					if ((new_lba <= old_lba) && ((new_lba + stoi(str_argv3)) > old_lba)) {
						// same lba (range)
					}
					else {
						v.push_back(tmp);
					}
				}
			}
			else if (old_cmd == "E") {
				// new command : write
				if (cmd == "W") {
					// erase after write
					v.push_back(tmp);
				}
				// new command : erase
				else if (cmd == "E") {
					int old_size = stoi(old_arg3);
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
								new_lba = old_lba;
								str_argv3 = std::to_string(old_size);
							}
							else {
								if (((new_lba + new_size) - old_lba) <= 10) {
									new_lba = old_lba;
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

		input_str = cmd + " " + std::to_string(new_lba) + " " + str_argv3;

		v.push_back(input_str);

		readFromWriteBuffer.close();
	}
	else {
		std::cout << "Failed to open file for reading." << std::endl;
		return -1;
	}

	count = v.size();

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
	if (ssd->updateWriteBuffer("W", address, data) == false)
		ssd->write(address, data);
}

EraseCommand::EraseCommand(SSD* ssd, int startAddress, int endAddress)
	: ssd(ssd), startAddress(startAddress), endAddress(endAddress) {}

void EraseCommand::execute() {
	if (ssd->updateWriteBuffer("E", startAddress, std::to_string(endAddress)) == false)
		ssd->erase(startAddress, endAddress);
}

FlushCommand::FlushCommand(SSD* ssd)
	: ssd(ssd) {}

void FlushCommand::execute() {
	ssd->flushWriteBuffer();
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