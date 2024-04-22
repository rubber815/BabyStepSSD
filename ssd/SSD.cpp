#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "../ssd/INAND.cpp"
#include "../ssd/SSD.hpp"


class SSD {
public:
	enum class Command
	{
		READ = 0,
		WRITE,
		ERASE,
		INVALIDCMD
	};

	void selectNAND(INAND* nand) {
		nand_ = nand;
	}
	void write(int lba, std::string value) {
		try {
			checkLbaRange(lba);
			checkValue(value);
		}
		catch (std::exception e) {
			return;
		}

		nand_->write(lba, value);
	}
	std::string read(int lba) {
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
	void erase(int lba, int size) {
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

private:
	INAND* nand_;

	void checkLbaRange(int lba) {
		if (lba < MIN_LBA || lba > MAX_LBA)
			throw std::invalid_argument(ERR::INVALID_LBA);
	}

	void checkEraseSize(int size) {
		if (size > 10)
			throw std::invalid_argument(ERR::OVER_ERASE_SIZE);
	}

	void checkValue(std::string value) {
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

	void writeToResultFile(std::string value) {
		std::ofstream outputFile(RESULT_FILE_NAME);
		if (!outputFile) {
			std::cout << ERR::OPEN_WRITING_FILE_FAIL << std::endl;
		}
		else {
			outputFile << value << std::endl;
			outputFile.close();
		}
	}
};

// Interface for Command
class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
	virtual std::string getResult() { return ""; };
};

// Concrete Command for Read operation
class ReadCommand : public Command {
private:
	SSD* ssd;
	int address;

	std::string result;
public:
	ReadCommand(SSD* ssd, int address) : ssd(ssd), address(address) {}

	void execute() override {
		result = ssd->read(address);
	}

	std::string getResult() {
		return result;
	}
};

// Concrete Command for Write operation
class WriteCommand : public Command {
private:
	SSD* ssd;
	int address;
	std::string data;
public:
	WriteCommand(SSD* ssd, int address, const std::string& data) : ssd(ssd), address(address), data(data) {}

	void execute() override {
		ssd->write(address, data);
	}
};

// Concrete Command for Erase operation
class EraseCommand : public Command {
private:
	SSD* ssd;
	int startAddress;
	int endAddress;
public:
	EraseCommand(SSD* ssd, int startAddress, int endAddress) : ssd(ssd), startAddress(startAddress), endAddress(endAddress) {}

	void execute() override {
		ssd->erase(startAddress, endAddress);
	}
};

// Invoker class
class SSDInvoker {
private:
	Command* command;
public:
	void setCommand(Command* cmd) {
		command = cmd;
	}

	void executeCommand() {
		if (command)
			command->execute();
		else
			std::cout << ERR::NO_COMMAND_SET;
	}

	std::string getResult() {
		return command->getResult();
	}
};