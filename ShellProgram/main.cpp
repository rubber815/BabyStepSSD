#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For system() function

bool verifyCommandFormat(const std::string& command) {
	// TODO
	return true;
}

void help() {
	std::cout << std::endl;
	std::cout << "************************************" << std::endl;
	std::cout << "* [ShellProgram commands help]" << std::endl;
	std::cout << "* exit: terminate shell" << std::endl;
	std::cout << "* help: print the usage for each command" << std::endl;
	std::cout << "* write: write to SSD" << std::endl;
	std::cout << "*		format: write <LBA> <VALUE>" << std::endl;
	std::cout << "* read: read from SSD" << std::endl;
	std::cout << "*		format: read <LBA>" << std::endl;
	std::cout << "* fullwrite: write from LBA numbers 0 to 99." << std::endl;
	std::cout << "*		format: fullwrite <VALUE>" << std::endl;
	std::cout << "* fullread: read from LBA numbers 0 to 99." << std::endl;
	std::cout << "*		format: fullread" << std::endl;
	std::cout << "*----------------------------------" << std::endl;
	std::cout << "* LBA: 0 ~ 99                      " << std::endl;
	std::cout << "* VALUE: 0x00000000 ~ 0xFFFFFFFF   " << std::endl;
	std::cout << "***********************************" << std::endl;
	std::cout << std::endl;
}
bool exit() {
	std::cout << "Exiting the program..." << std::endl;
	return false;
}

bool write() {
	// TODO
	//system("");
	return false;
}
bool read() {
	// TODO
	//system("");
	return false;
}
bool fullWrite() {
	// TODO
	//system("");
	return false;
}
bool fullRead() {
	// TODO
	//system("");
	return false;
}

bool testApp1() {
	// TODO
	//system("");
	return false;
}

bool testApp2() {
	// TODO
	//system("");
	return false;
}

int main() {
	std::string command, operation, lba, value;

	while (true) {
		std::cout << "Welcome to ShellProgram!!: "; // Updated prompt
		std::getline(std::cin, command);

		if (!(verifyCommandFormat(command))) {
			std::cout << "FATAL: INVALID_FORMAT!" << std::endl;
			continue;
		}

		// parsing commands
		std::istringstream iss(command);
		iss >> operation;

		// Shell commands
		if (operation == "exit") {
			exit();
			break;
		}

		if (operation == "help") {
			help();
			continue;
		}

		/*RW commands*/
		if (operation == "write") {
			iss >> lba;
			iss >> value;
			write();
		}
		else if (operation == "read") {
			iss >> lba;
			read();
		}
		else if (operation == "fullwrite") {
			iss >> value;
			fullWrite();
		}
		else if (operation == "fullread") {
			fullRead();
		}
		/*TestApp commands*/
		else if (operation == "testapp1") {
			testApp1();
		}
		else if (operation == "testapp2")
			testApp2();

		// for reference: Execute the .exe file
		//system("SSDProject W 1 0x11111111");
		//system("SSDProject W 2 0x22222222");
		//system("SSDProject R 1");
	}
}