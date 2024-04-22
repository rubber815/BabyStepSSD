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

bool FullRead10Compare() {
	const std::string input1 = "0xABCDEFAB";
	const std::string input2 = "0xEFFFFFFF";
	const std::string input3 = "0x12345678";

	// Initial Write
	for (int lba = 0; lba < 30; lba++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lba);
		cmd += " " + input1;
		system(cmd.c_str());
	}

	for (int lba = 30; lba < 70; lba++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lba);
		cmd += " " + input2;
		system(cmd.c_str());
	}

	for (int lba = 70; lba < 100; lba++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lba);
		cmd += " " + input3;
		system(cmd.c_str());
	}

	// Compare
	for (int lba = 0; lba < 30; lba++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lba);
		system(cmd.c_str());

		std::string read_val = readFromSSDResultFile();
		if (read_val != input1)
			return false;
	}
	for (int lba = 30; lba < 70; lba++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lba);
		system(cmd.c_str());

		std::string read_val = readFromSSDResultFile();
		if (read_val != input2)
			return false;
	}
	for (int lba = 70; lba < 100; lba++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lba);
		system(cmd.c_str());

		std::string read_val = readFromSSDResultFile();
		if (read_val != input3)
			return false;
	}
	return true;
}


int main(int argc, char* argv[]) {
	bool ret = FullRead10Compare();
	if (ret) {
		writeToTestResultFile("Pass");
	}
	else {
		writeToTestResultFile("FAIL!");
	}
}