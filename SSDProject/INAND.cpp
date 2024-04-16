#pragma once
#include <string>

class INAND {
public:
	virtual void write(int lba, std::string value) = 0;
	virtual std::string read(int lba) = 0;
private:
};