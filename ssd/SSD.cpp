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