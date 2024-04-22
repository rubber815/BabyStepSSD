#include <iostream>
#include <string>
#include <fstream>
#include <vector>


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

bool Write10AndCompare() {
	const std::vector<std::string> inputs 
		= { "0xABCDEFAB" , "0xEFFFFFFF" , "0x12345678" , 
		"0x12345673" , "0xEFFFFEFF" , "0xABCDCFAB" ,
	"0xABCDEFAB", "0xDBCDEFAB", "0xABADEFAB" ,"0xAB0DEFAB"};

	const std::vector<int> lbas = {1, 7, 30, 50, 33, 55, 87, 90, 27, 4};
	
	// Initial Write
	for (int idx = 0; idx < 10; idx++) {
		std::string cmd = "ssd W ";
		cmd += std::to_string(lbas[idx]);
		cmd += " " + inputs[idx];
		system(cmd.c_str());
	}
	
	for (int idx = 0; idx < 10; idx++) {
		std::string cmd = "ssd R ";
		cmd += std::to_string(lbas[idx]);
		system(cmd.c_str());

		std::string read_val = readFromSSDResultFile();
		if (read_val != inputs[idx])
			return false;
	}

	return true;
}


int main(int argc, char* argv[]) {
	bool ret = Write10AndCompare();
	if (ret) {
		writeToTestResultFile("Pass");
	}
	else {
		writeToTestResultFile("FAIL!");
	}
}