#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "SSD.cpp"
#include "BabyStepNAND.cpp"

SSD* initSSD() {
    return new SSD();
}

bool mainWrite(SSD* ssd, int lba, std::string value) {
    ssd->write(lba, value);

    return true;
}
bool mainRead(SSD* ssd, int lba) {
    std::string readData = ssd->read(lba);

    return true;
}

int main(int argc, char* argv[]) {
    std::string cmd = argv[1];

    SSD* babyStepSSD = initSSD();
    BabyStepNand* babyStepNand = new BabyStepNand();
    babyStepSSD->selectNAND(babyStepNand);

    if (cmd == "R") {
        std::string lba = argv[2];
        mainRead(babyStepSSD, stoi(lba));
    }
    if (cmd == "W") {
        std::string lba = argv[2];
        mainWrite(babyStepSSD, stoi(lba), argv[3]);
    }

    delete babyStepNand;
    delete babyStepSSD;

    return 0;
}