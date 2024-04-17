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
		system("C:\\Users\\User\\source\\repos\\SSDProject\\x64\\Debug\\SSDProject.exe W 1 0x11111111");
		system("C:\\Users\\User\\source\\repos\\SSDProject\\x64\\Debug\\SSDProject.exe W 2 0x22222222");
		system("C:\\Users\\User\\source\\repos\\SSDProject\\x64\\Debug\\SSDProject.exe R 1");
		//system("C:\\Users\\User\\source\\repos\\SSDProject\\x64\\Debug\\SSDProject.exe W 2 0x22222222");
	}
}