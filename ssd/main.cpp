#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "SSD.cpp"
#include "BabyStepNAND.cpp"

int main(int argc, char* argv[]) {
    std::string cmd = argv[1];

    SSD babyStepSSD;
    BabyStepNand babyStepNand;
    babyStepSSD.selectNAND(&babyStepNand);
    
    SSDInvoker invoker;

    if (cmd == "R") {
        int address = std::atoi(argv[2]);
        invoker.setCommand(new ReadCommand(&babyStepSSD, address));
    } 
    else if (cmd == "W") {
        int address = std::atoi(argv[2]);
        std::string data = argv[3];
        invoker.setCommand(new WriteCommand(&babyStepSSD, address, data));
    }
    else if (cmd == "E") {
        int startAddress = std::atoi(argv[2]);
        int endAddress = std::atoi(argv[3]);
        invoker.setCommand(new EraseCommand(&babyStepSSD, startAddress, endAddress));
    }

    invoker.executeCommand();

    return 0;
}