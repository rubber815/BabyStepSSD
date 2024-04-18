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
		std::string cmd = argv[1];
		searchWriteBuffer(argv);
		if (cmd == "R") {

			//if (getdata() == true)
				//writeToResultFile(value);
				return true;
		}
		else if (cmd == "W") {

		}
		else if (cmd == "E") {

		}
		//flush
		return true;
		//return false;
	}
	void flushWriteBuffer(void) {

	}
	void searchWriteBuffer(char* argv[]) {
		std::vector<std::string> v;
		int count;

		std::ifstream readFromWriteBuffer;
		readFromWriteBuffer.open(WRITE_BUFFER_FILE_NAME);

		if (readFromWriteBuffer.is_open()) {
			std::string tmp;
			std::string input_str;
			getline(readFromWriteBuffer, tmp);
			count = stoi(tmp);
			std::cout << count << std::endl;
			for (int i = 0; i < count; i++) {
				getline(readFromWriteBuffer, tmp);

				int lba = tmp[2] - '0';

				if (tmp[0] == 'W') {
					//if (argv[1] == "W") {
						if (lba == atoi(argv[2])) {
							//std::cout << tmp << std::endl;
						}
						else {
							std::cout << tmp << std::endl;
							v.push_back(tmp);
						}
					//}
					/*else if (argv[1] == "E") {
						if ((atoi(argv[2]) <= lba) && ((atoi(argv[2]) + atoi(argv[3])) > lba)) {

						}
						else {
							v.push_back(tmp);
						}
					}*/
				}
				else if (tmp[0] == 'E') {
					if (argv[1] == "W") {
						//To-Do
					}
					else if (argv[1] == "E") {
						//To-Do
					}
					v.push_back(tmp);
				}
			}

			input_str = argv[1];
			input_str = input_str + " ";
			input_str = input_str + argv[2];
			input_str = input_str + " ";
			input_str = input_str + argv[3];

			v.push_back(input_str);

			readFromWriteBuffer.close();
		}
		else
			return;

		count = v.size();

		std::ofstream outputFile(WRITE_BUFFER_FILE_NAME);
		if (!outputFile) {
			std::cout << "Failed to open file for writing." << std::endl;
		}
		else {
			if (v.size() == 10) {
				//flush()
				//v.clear();
			}
			outputFile << count << std::endl;
			for (int i = 0; i < count; i++) {
				outputFile << v[i] << std::endl;
				std::cout << v[i] << std::endl;
			}
			outputFile.close();
		}

		return;
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