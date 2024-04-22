#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "SSD.hpp"
#include "BabyStepNAND.cpp"

std::unordered_map<std::string, SSD::Command> cmdMap = {
	{"R", SSD::Command::READ},
	{"W", SSD::Command::WRITE},
	{"E", SSD::Command::ERASE},
	{"F", SSD::Command::FLUSH},
};

int main(int argc, char* argv[]) {
	SSD babyStepSSD;
	BabyStepNand babyStepNand;
	babyStepSSD.selectNAND(&babyStepNand);

	SSDInvoker invoker;

	std::string cmd = argv[1];
	switch (cmdMap[cmd])
	{
	case SSD::Command::READ:
		invoker.setCommand(new ReadCommand(&babyStepSSD, std::atoi(argv[2])));
		break;
	case SSD::Command::WRITE:
		invoker.setCommand(new WriteCommand(&babyStepSSD, std::atoi(argv[2]), argv[3]));
		break;
	case SSD::Command::ERASE:
		invoker.setCommand(new EraseCommand(&babyStepSSD, std::atoi(argv[2]), std::atoi(argv[3])));
		break;
	case SSD::Command::FLUSH:
		invoker.setCommand(new FlushCommand(&babyStepSSD));
		break;
	default:
		break;
	}

	if (babyStepSSD.updateWriteBuffer(argv))
		return 0;
	invoker.executeCommand();

	return 0;
}