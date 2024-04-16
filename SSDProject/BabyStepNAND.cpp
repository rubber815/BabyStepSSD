#include "INAND.cpp"
#include <fstream>
#include <vector>

class BabyStepNand : public INAND {
public:
	void write(int lba, std::string value) override;
	std::string read(int lba) override;

private:

};

void
BabyStepNand::write(int lba, std::string value) {
	// To-do
	/*
	vector<std::string> nandData = { "0x12345678", "0x77777777" };
	int len = static_cast<int>(nandData.size());

	ofstream writeToNand;
		writeToNand.open("nand.txt");

		for (int i = 0; i < len; i++) {
			std::string tmp = nandData[i];
			if (i != len - 1) {
				tmp += "\n";
			}

			writeToNand.write(tmp.c_str(), tmp.size());
		}
		writeToNand.close();
	*/
}

std::string
BabyStepNand::read(int lba) {
	// To-do
	std::string data;

	return data;
}