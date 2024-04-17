#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "SSD.cpp"
#include "BabyStepNAND.cpp"

SSD* initSSD() {
	return new SSD();
}

void terminateSSD(SSD* ssd) {
	delete ssd;
}

bool mainHelp() {
	// TODO
	return false;
}
bool mainExit() {
	// TODO
	return false;
}
bool mainWrite() {
	// TODO
	return false;
}
bool mainRead() {
	// TODO
	return false;
}
bool mainFullWrite() {
	// TODO
	return false;
}
bool mainFullRead() {
	// TODO
	return false;
}

int main() {
	//FAKE_SSD ssd;
	std::string command, operation, value;
	int lba;

	SSD* babyStepSSD = initSSD();
	BabyStepNand* babyStepNand = new BabyStepNand();
	babyStepSSD->selectNAND(babyStepNand);

	while (true) {
		std::cout << "Welcome to BabyStepNand!!: "; // Updated prompt
		std::getline(std::cin, command);

		if (!(babyStepSSD->verifyCommandFormat(command))) {
			std::cout << "FATAL: INVALID_FORMAT!" << std::endl;
			continue;
		}
		
		// parsing commands
		std::istringstream iss(command);
		iss >> operation >> lba;

		// TODO: mainExit()
		// Added exit condition
		if (operation == "exit") {
			terminateSSD(babyStepSSD);
			std::cout << "Exiting the program." << std::endl;
			break;
		}

		// TODO: mainHelp()
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
			// TODO: mainRead();

			// TODO: will be removed
			std::cout << "Write successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Value]: " << value << std::endl;
		}
		else if (operation == "read") {
			// TODO: mainWrite();

			// TODO: will be removed
			std::cout << "Read successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Read Value]: 0x1111" << std::endl;
		}
		else if (operation == "fullwrite") {
			// TODO: mainFullWrite();

			// TODO: will be removed
			std::cout << "FullWrite successful." << std::endl;
			std::cout << "[LBA]: 0 ~ 99 [Write Value]: 0x1111" << std::endl;
		}
		else if (operation == "fullread") {
			// TODO: mainFullRead();

			// TODO: will be removed
			std::cout << "FullRead successful." << std::endl;
			std::cout << "[LBA]: 0 ~ 99 [Read Value]: 0x1111" << std::endl;
		}
	}

	return 0;
}