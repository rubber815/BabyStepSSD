#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "../SSDProject/INAND.cpp"
#include <stdexcept>
#include <vector>

class SSD {
public:
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
	bool updateWriteBuffer(char* argv[]) {
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
	bool flushWriteBuffer(void) {
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
	int AddCmdWriteBuffer(char* argv[]) {
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
	bool ReadWriteBuffer(char* argv[]) {

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

	// TODO
	bool verifyCommandFormat(const std::string& command) {
		std::istringstream iss(command);
		std::string op, lba, endlba, val, size;

		iss >> op;

		/*exit: terminate shell*/
		if (op == "exit") {
			return true;
		}

		/*help: print the usage for each command.*/
		if (op == "help") {
			return true;
		}

		/*RW op format verification*/
		if (op == "fullwrite") {
			iss >> val;
			try {
				checkValue(val);
			}
			catch (std::invalid_argument e) {
				std::cout << e.what() << std::endl;
				return false;
			}
			return true;
		}
		
		if(op == "fullread") {
			// TODO: detail format check
			return true;
		}

		if (op == "write") {
			iss >> lba >> val;

			try {
				checkLbaRange(stoi(lba));
				checkValue(val);
			}
			catch (std::exception e) {
				return false;
			}
			return true;
		}
		
		if (op == "read") {
			iss >> lba;

			try {
				checkLbaRange(stoi(lba));
			}
			catch (std::exception e) {
				return false;
			}

			return true;
		}

		if (op == "erase") {
			iss >> lba;
			iss >> size;

			try {
				checkLbaRange(stoi(lba));
				checkLbaRange(stoi(lba) + stoi(size) - 1);
				checkEraseSize(stoi(size));
			}
			catch (std::exception e) {
				return false;
			}
			return true;
		}

		if (op == "erase_range") {
			iss >> lba;
			iss >> endlba;

			try {
				checkLbaRange(stoi(lba));
				checkLbaRange(stoi(endlba) - 1);
				checkEraseSize(stoi(endlba) - stoi(lba));
			}
			catch (std::exception e) {
				return false;
			}
			return true;
		}

		/*Test App format verification*/
		if (op == "testapp1") {
			// TODO: detail format check
			return true;
		}

		if (op == "testapp2") {
			// TODO: detail format check
			return true;
		}

		/*RW op formant not verified!!!*/
		return false;
	}
private:
	INAND * nand_;

	const int MIN_LBA = 0;
	const int MAX_LBA = 99;
	const int VALUE_LENGTH = 10;
	const std::string PREFIX_VALUE = "0x";
	const std::string RESULT_FILE_NAME = "result.txt";
	const std::string WRITE_BUFFER_FILE_NAME = "writebuffer.txt";

	void checkLbaRange(int lba) {
		if (lba < MIN_LBA || lba > MAX_LBA)
			throw std::invalid_argument("lba is incorrect");
	}

	void checkEraseSize(int size) {
		if (size > 10)
			throw std::invalid_argument("over size");
	}

	void checkValue(std::string value) {
		if (value.length() != VALUE_LENGTH)
			throw std::invalid_argument("VALUE has a maximum length of 10.");

		if (value.substr(0, 2) != PREFIX_VALUE)
			throw std::invalid_argument("VALUE must be entered as 0x in hexadecimal.");

		for (int i = 2; i < value.length(); i++) {
			if (!(value[i] >= '0' && value[i] <= '9')
				&& !(value[i] >= 'A' && value[i] <= 'F'))
				throw std::invalid_argument("VALUE can be entered from 0 to 9 or A to F.");
		}
	}

	void writeToResultFile(std::string value) {
		std::ofstream outputFile(RESULT_FILE_NAME);
		if (!outputFile) {
			std::cout << "Failed to open file for writing." << std::endl;
		}
		else {
			outputFile << value << std::endl;
			outputFile.close();
		}
	}
};