#pragma once

#include <string>

class INAND {
public:
	virtual void erase(int lba, int size) = 0;
	virtual void write(int lba, std::string value) = 0;
	virtual std::string read(int lba) = 0;
private:
};