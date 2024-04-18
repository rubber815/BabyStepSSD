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

    // Check Write Buffer
    // To-do
    if (babyStepSSD.updateWriteBuffer(argv))
        return 0;

    if (cmd == "R")
        babyStepSSD.read(atoi(argv[2]));

    if (cmd == "W")
        babyStepSSD.write(atoi(argv[2]), argv[3]);

    if (cmd == "E")
        babyStepSSD.erase(atoi(argv[2]), atoi(argv[3]));

    /*if (cmd == "F")
        babyStepSSD.flushWriteBuffer();*/

    return 0;
}