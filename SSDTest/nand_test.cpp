#pragma once

#include <gtest/gtest.h>

#include <stdexcept>
#include "../SSDProject/BabyStepNAND.cpp"

using namespace testing;

/*
1. W 20 0x1289CDEF
2. R 20 --> 0x1289CDEF
3. R 19 --> 0x00000000
4. W 10 0xFF1100AA
5. R 10 --> 0xFF1100AA
*/

TEST(NANDTest, Nand_single_write_read_test) {
	BabyStepNand bsn;
	bsn.write(0x3, "0x333");
	EXPECT_EQ("0x333", bsn.read(0x3));
}

// TODO