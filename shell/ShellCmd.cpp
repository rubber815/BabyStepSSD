#pragma once
#include <string>
#include "Logger.hpp"

#define APP_NAME "ssd "

const int MAX_LBA = 99;
const std::string RESULT_FILE_NAME = "result.txt";

// Interface for ShellCmd
class ShellCmd {
public:
	virtual ~ShellCmd() {}
	virtual void execute() = 0;
	virtual std::string getResult() { return ""; };
protected:
	std::string makeSSDCommand(std::string cmd, std::string arg1, std::string arg2) {
		std::string result;
		result = APP_NAME + cmd + " " + arg1 + " " + arg2;

		return result;
	}
	std::string readFromResultFile(std::string fileName) {
		std::ifstream inputFile(fileName);
		std::string value;
		if (!inputFile.is_open()) {
			std::cout << "Failed to open file for reading." << std::endl;
			return value;
		}
		std::getline(inputFile, value);
		inputFile.close();
		return value;
	}
private:

};

class ShellReadCommand : public ShellCmd {
private:
	std::string m_lba;
	std::string result;
public:
	ShellReadCommand(std::istringstream& iss) {
		iss >> m_lba;
	}

	void execute() override {
		std::string str = makeSSDCommand("R", m_lba, "").c_str();
		LOG_FUNCTION_CALL(str);
		system(str.c_str());
	}

	std::string getResult() {
		return result;
	}
};

class ShellFullReadCommand : public ShellCmd {
private:
	bool m_compare;
	std::string m_comp_val;
	std::string result;
public:
	ShellFullReadCommand(bool compare, std::string comp_val)
		: m_compare(compare), m_comp_val(comp_val)
	{
	}

	void execute() override {
		LOG_FUNCTION_CALL("");

		for (int lba = 0; lba < (MAX_LBA + 1); lba++) {
			system(makeSSDCommand("R", std::to_string(lba), "").c_str());

			std::string read_val = readFromResultFile(RESULT_FILE_NAME);
			std::cout << read_val << std::endl;

			if (m_compare) {
				if (read_val != m_comp_val) {
					std::cout << "Read Compare Fail! in LBA: " << lba << std::endl;
					std::cout << "Expected: " + m_comp_val + ", Result: " + read_val << lba << std::endl;
					return;
				}
			}
		}
	}

	std::string getResult() {
		return result;
	}
};

class ShellWriteCommand : public ShellCmd {
private:
	std::string m_lba;
	std::string m_value;
public:
	ShellWriteCommand(std::istringstream& iss) {
		iss >> m_lba;
		iss >> m_value;
	}

	void execute() override {
		std::string str = makeSSDCommand("W", m_lba, m_value).c_str();
		LOG_FUNCTION_CALL(str);
		system(str.c_str());
	}
};

class ShellFullWriteCommand : public ShellCmd {
private:
	std::string m_value;
public:
	ShellFullWriteCommand(std::istringstream& iss) {
		iss >> m_value;
	}

	void execute() override {
		LOG_FUNCTION_CALL(m_value);
		for (int i = 0; i < (MAX_LBA + 1); i++)
			system(makeSSDCommand("W", std::to_string(i), m_value).c_str());
	}
};

class ShellEraseCommand : public ShellCmd {
private:
	std::string m_lba;
	std::string m_size;
public:
	ShellEraseCommand(std::istringstream& iss) {
		iss >> m_lba;
		iss >> m_size;
	}

	void execute() override {
		std::string str = makeSSDCommand("E", m_lba, m_size).c_str();
		LOG_FUNCTION_CALL(str);
		system(str.c_str());
	}
};

class ShellEraseRangeCommand : public ShellCmd {
private:
	std::string m_start_lba;
	std::string m_end_lba;
public:
	ShellEraseRangeCommand(std::istringstream& iss) {
		iss >> m_start_lba;
		iss >> m_end_lba;
	}

	void execute() override {
		int size = stoi(m_end_lba) - stoi(m_start_lba);
		std::string str = makeSSDCommand("E", m_start_lba, std::to_string(size)).c_str();
		LOG_FUNCTION_CALL(str);
		system(str.c_str());
	}
};

class ShellFlushCommand : public ShellCmd {
private:
public:
	ShellFlushCommand() {
	}

	void execute() override {
		LOG_FUNCTION_CALL("F");
		system(makeSSDCommand("F", "", "").c_str());
	}
};

// Invoker class
class ShellCmdInvoker {
private:
	ShellCmd* shell_command;
public:
	void setCommand(ShellCmd* cmd) {
		shell_command = cmd;
	}

	void executeCommand() {
		if (shell_command)
			shell_command->execute();
		else
			std::cout << "ERR::NO_COMMAND_SET";
	}

	std::string getResult() {
		return shell_command->getResult();
	}
};