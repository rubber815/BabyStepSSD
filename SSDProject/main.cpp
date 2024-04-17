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
bool mainFullWrite(SSD* babyStepSSD, std::string value) {
	for (int i = 0; i < 100; i++)
		babyStepSSD->write(i, value);

	return true;
}
bool mainFullRead(SSD* babyStepSSD) {
	for (int i = 0; i < 100; i++)
		std::cout << babyStepSSD->read(i) << std::endl;

	return true;
}

bool mainTestApp1(SSD* ssd) {
	/*	Full Write + ReadCompare
	1. fullwrite
	2. fullread + readcompare
	*/
	const std::string input1 = "0xABCDEFAB";
	for (int lba = 0; lba < 100; lba++) {
		ssd->write(lba, input1);

		std::cout << "Write: " << lba << ", Value:" << input1 << std::endl;
	}
	for (int lba = 0; lba < 100; lba++) {
		std::string ret = ssd->read(lba);
		std::cout << "Read " << lba << ", Value:" << ret << std::endl;
		if (ret != input1)
			return false;
	}

	return true;
}

void mainTestApp2(SSD* ssd) {
	const int agingCnt = 30;
	const int maxLba = 5;
	const std::string value1 = "0xAAAABBBB";
	const std::string value2 = "0x12345678";
	for (int i = 0; i < agingCnt; i++) {
		for (int lba = 0; lba <= maxLba; lba++)
			ssd->write(lba, value1);
	}

	for (int lba = 0; lba <= maxLba; lba++)
		ssd->write(lba, value2);

	for (int lba = 0; lba <= maxLba; lba++) {
		std::string retValue = ssd->read(lba);
		if (retValue == value2) {
			std::cout << "TestApp2: Read compare after "
				"write aging: Passed, LBA: " << lba
				<< ", Expected: " << value2 << ", Actual: "
				<< retValue << std::endl;
		}
		else {
			std::cout << "TestApp2: Read compare after "
				"write aging: Failed, LBA: " << lba
				<< ", Expected: " << value2 << ", Actual: "
				<< retValue << std::endl;
		}
	}
}

int main() {
	//FAKE_SSD ssd;
	std::string command, operation, lba, value;

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
		iss >> operation;

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
			iss >> lba;
			iss >> value;
			// TODO: mainRead();

			// TODO: will be removed
			std::cout << "Write successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Value]: " << value << std::endl;
		}
		else if (operation == "read") {
			iss >> lba;
			// TODO: mainWrite();

			// TODO: will be removed
			std::cout << "Read successful." << std::endl;
			std::cout << "[LBA]: " << lba << "[Read Value]: 0x1111" << std::endl;
		}
		else if (operation == "fullwrite") {
			iss >> value;
			mainFullWrite(babyStepSSD, value);
		}
		else if (operation == "fullread") {
			mainFullRead(babyStepSSD);
		}
		else if (operation == "testapp1") {
			// TODO: mainTestApp1();
			// TODO: will be removed
			mainTestApp1(babyStepSSD);
			std::cout << "TestApp1 successful." << std::endl;
		}
		else if (operation == "testapp2")
			mainTestApp2(babyStepSSD);
	}

	return 0;
}