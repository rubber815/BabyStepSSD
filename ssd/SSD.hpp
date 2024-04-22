#pragma once
#include <string>
#include "INAND.cpp"

const int MIN_LBA = 0;
const int MAX_LBA = 99;
const int VALUE_LENGTH = 10;
const std::string PREFIX_VALUE = "0x";
const std::string RESULT_FILE_NAME = "result.txt";

namespace ERR {
	const std::string INVALID_LBA = "lba is incorrect";
	const std::string OVER_ERASE_SIZE = "over size";
	const std::string INVALID_VALUE_LENGTH = "VALUE has a maximum length of 10.";
	const std::string INVALID_VALUE_PREFIX = "VALUE must be entered as 0x in hexadecimal.";
	const std::string INVALID_VALUE_CHARACTER = "VALUE can be entered from 0 to 9 or A to F.";

	const std::string OPEN_WRITING_FILE_FAIL = "Failed to open file for writing.";

	const std::string NO_COMMAND_SET = "No command set!\n";
}

class SSD {
public:
	enum class Command
	{
		READ = 0,
		WRITE,
		ERASE,
		INVALIDCMD
	};

	void selectNAND(INAND* nand);

	void write(int lba, std::string value);

	std::string read(int lba);

	void erase(int lba, int size);

private:
	INAND* nand_;

	void checkLbaRange(int lba);

	void checkEraseSize(int size);

	void checkValue(std::string value);

	void writeToResultFile(std::string value);
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
	ReadCommand(SSD* ssd, int address);

	void execute() override;

	std::string getResult();
};

// Concrete Command for Write operation
class WriteCommand : public Command {
private:
	SSD* ssd;
	int address;
	std::string data;
public:
	WriteCommand(SSD* ssd, int address, const std::string& data);

	void execute() override;
};

// Concrete Command for Erase operation
class EraseCommand : public Command {
private:
	SSD* ssd;
	int startAddress;
	int endAddress;
public:
	EraseCommand(SSD* ssd, int startAddress, int endAddress);

	void execute() override;
};

// Invoker class
class SSDInvoker {
private:
	Command* command;
public:
	void setCommand(Command* cmd);

	void executeCommand();

	std::string getResult();
};