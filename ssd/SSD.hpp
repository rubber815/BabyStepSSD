#pragma once
#include <string>

const int MIN_LBA = 0;
const int MAX_LBA = 99;
const int VALUE_LENGTH = 10;
const std::string PREFIX_VALUE = "0x";
const std::string RESULT_FILE_NAME = "result.txt";

namespace ERR {
	const std::string INVALID_LBA = "lba is incorrect";
	const std::string OVER_ERASE_SIZE = "over size";
	const std::string INVALID_VALUE_LENGTH = "VALUE has a maximum length of 10.";
	const std::string INVALID_VALUE_PREFIX = "VALUE must be entered as 0x in hexadecimal.";
	const std::string INVALID_VALUE_CHARACTER = "VALUE can be entered from 0 to 9 or A to F.";

	const std::string OPEN_WRITING_FILE_FAIL = "Failed to open file for writing.";

	const std::string NO_COMMAND_SET = "No command set!\n";
}