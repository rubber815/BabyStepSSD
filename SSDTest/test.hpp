#pragma once
#include <string>

#include "../ssd/BabyStepNAND.cpp"
#include "../ssd/SSD.hpp"

const std::string NORMAL_VALUE_ZERO = "0x00000000";
const std::string NORMAL_VALUE_AAAABBBB = "0xAAAABBBB";
const std::string NORMAL_VALUE_0000AAAA = "0x0000AAAA";
const std::string NORMAL_VALUE_1289CDEF = "0x1289CDEF";

const std::string INVALID_VALUE_ZERO_OVER_RANGE = "0x0000000000";
const std::string INVALID_VALUE_WRONG_PREFIX = "xx00000000";

const int NORMAL_LBA_ZERO = 0;
const int NORMAL_LBA_NON_ZERO = 7;
const int NORMAL_LBA_10 = 10;
const int NORMAL_LBA_30 = 30;

const int INVALID_LBA_UNDER_RANGE = -1;
const int INVALID_LBA_OVER_RANGE = 101;

const int CALLED_TIME_ZERO = 0;
const int CALLED_TIME_ONE = 1;

const std::string EMPTY_STRING = "";