#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For system() function

int main() {
	std::string command, operation;

	while (true) {
		std::cout << "Welcome to ShellProgram!!: "; // Updated prompt
		std::getline(std::cin, command);

		// parsing commands
		std::istringstream iss(command);
		iss >> operation;

		// Execute the .exe file
		system("SSDProject W 1 0x11111111");
		system("SSDProject W 2 0x22222222");
		system("SSDProject R 1");
	}
}