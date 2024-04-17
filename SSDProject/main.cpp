#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

bool verifyCommandFormat(const std::string& command) {
	std::istringstream iss(command);
	std::string op;
	int lba;

	/*exit: terminate shell*/
	if (command == "exit") {
		return true;
	}

	/*help: print the usage for each command.*/
	if (command == "help") {
		return true;
	}

	/*RW op format verification*/
	if (command == "fullwrite" || command == "fullread") {
		// TODO: detail format check
		return true;
	}

	if (!(iss >> op >> lba)) {
			return false;
	}
	if (op == "write" || op == "read") {

		// TODO: detail format check
		return true;
	}


	/*RW op formant not verified!!!*/
	return false;
}

int main() {
	//FAKE_SSD ssd;
	std::string command, operation;
	int lba, value;

	while (true) {
		std::cout << "Welcome to BabyStepNand!!: "; // Updated prompt
		std::getline(std::cin, command);

		if (!verifyCommandFormat(command)) {
			std::cout << "FATAL: INVALID_FORMAT!" << std::endl;
			continue;
		}

		std::istringstream iss(command);
		iss >> operation >> lba;

		// Added exit condition
		if (operation == "exit") {
			std::cout << "Exiting the program." << std::endl;
			break;
		}

		if (operation == "help") {
			std::cout << std::endl;
			std::cout << "[BabyStepNand commands help]" << std::endl;
			std::cout << "- exit: terminate shell" << std::endl;
			std::cout << "- help: print the usage for each command" << std::endl;
			std::cout << "- write: write to SSD" << std::endl;
			std::cout << "- read: read from SSD" << std::endl;
			std::cout << "- fullwrite: write from LBA numbers 0 to 99." << std::endl;
			std::cout << "- fullread: read from LBA numbers 0 to 99." << std::endl;
			std::cout << std::endl;
			continue;
		}

		/*RW commands*/
		if (operation == "write") {
			iss >> value;
			// Pseudo code
			// TODO: ssd.write(lba, value);
			std::cout << "Write successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Value]: " << value << std::endl;
		}
		else if (operation == "read") {
			// Pseudo code
			//int read_value = ssd.read(lba);
			//if (read_value != -1)
			//	cout << "Read successful. Value at LBA " << lba << ": " << read_value << endl;
			//else
			//	cout << "Read failed. LBA " << lba << " is empty." << endl;
			std::cout << "Read successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Read Value]: 0x1111" << std::endl;
		}
		else if (operation == "fullwrite") {
			// TODO
			std::cout << "FullWrite successful." << std::endl;
			std::cout << "[LBA]: 0 ~ 99 [Write Value]: 0x1111" << std::endl;
		}
		else if (operation == "fullread") {
			// TODO
			std::cout << "FullRead successful." << std::endl;
			std::cout << "[LBA]: 0 ~ 99 [Read Value]: 0x1111" << std::endl;
		}
	}

	return 0;
}