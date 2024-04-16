#include "INAND.cpp"
#include <fstream>
#include <vector>

class BabyStepNand : public INAND {
public:
	// To-do (initialize buffer)
	// void BabyStepNand();

	void write(int lba, std::string value) override;
	std::string read(int lba) override;

private:
	std::vector<std::string> buffer;
};

void BabyStepNand::write(int lba, std::string value) {
	// Read All NAND
	std::ifstream readFromNand;
	readFromNand.open("nand.txt");

	if (readFromNand.is_open()) {
		this->buffer.clear();

		while (!readFromNand.eof()) {
			std::string tmp;
			getline(readFromNand, tmp);
			this->buffer.push_back(tmp);
		}
	}


	// Modify NAND data
	buffer[lba] = value;


	// Write All NAND
	int len = static_cast<int>(buffer.size());

	std::ofstream writeToNand;
	writeToNand.open("nand.txt");

	for (int i = 0; i < len; i++) {
		std::string tmp = buffer[i];
		if (i != len - 1) {
			tmp += "\n";
		}
		writeToNand.write(tmp.c_str(), tmp.size());
	}
	writeToNand.close();
}

std::string BabyStepNand::read(int lba) {
	// To-do
	std::string data;

	return data;
}