#include <iostream>
#include <string>
#include <fstream>


std::string readFromSSDResultFile() {
	const std::string RESULT_FILE_NAME = "result.txt";
	std::ifstream inputFile(RESULT_FILE_NAME);
	std::string value;

	if (!inputFile.is_open()) {
		std::cout << "Failed to open file for reading." << std::endl;
		return value;
	}
	std::getline(inputFile, value);

	inputFile.close();
	return value;

}

void writeToTestResultFile(std::string value) {
	const std::string RESULT_FILE_NAME = "testResult.txt";
	std::ofstream outputFile(RESULT_FILE_NAME);
	if (!outputFile) {
		std::cout << "Failed to open file for writing." << std::endl;
	}
	else {
		outputFile << value << std::endl;
		outputFile.close();
	}

}

bool testApp3() {
	/*	Full Write + ReadCompare
	1. fullwrite
	2. fullread + readcompare
	*/
	const std::string input1 = "0xABCDEFAB";
	for (int lba = 0; lba < 100; lba++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lba);
		cmd += " " + input1;
		system(cmd.c_str());
	}
	for (int lba = 0; lba < 100; lba++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lba);
		system(cmd.c_str());

		// Compare
		std::string read_val = readFromSSDResultFile();
		if (read_val != input1)
			return false;
	}
	return true;
}

int main(int argc, char* argv[]) {
	bool ret = testApp3();
	if (ret) {
		writeToTestResultFile("Pass");
	}
	else {
		writeToTestResultFile("FAIL!");
	}
}