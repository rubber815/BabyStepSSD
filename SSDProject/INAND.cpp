#pragma once

class INAND {
public:
	virtual void write(int lba, int value) = 0;
	virtual int read(int lba) = 0;
private:
};