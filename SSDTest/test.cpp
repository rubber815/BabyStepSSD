#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../SSDProject/INAND.cpp"
#include "../SSDProject/SSD.cpp"
#include <stdexcept>

using namespace testing;

/*
1. W 20 0x1289CDEF
2. R 20 --> 0x1289CDEF
3. R 19 --> 0x00000000
4. W 10 0xFF1100AA
5. R 10 --> 0xFF1100AA
*/

// mocking ±¸¼º
class MockNAND : public INAND {
public:
	MOCK_METHOD(void, write, (int, std::string), (override));
	MOCK_METHOD(std::string, read, (int), (override));
};

class SSDTest : public testing::Test {
protected:
	void SetUp() override {
		ssd_.selectNAND(&m_nand_);
	}
public:
	SSD ssd_;
	MockNAND m_nand_;
};

TEST_F(SSDTest, Read_lba_test) {
	EXPECT_CALL(m_nand_, write(0, "0x00000000"))
		.Times(1);
	ssd_.write(0, "0x00000000");
}

TEST_F(SSDTest, Read_lower_lba_fail_test) {
	EXPECT_CALL(m_nand_, write(-1, "0x00000000"))
		.Times(0);

	EXPECT_THROW(ssd_.write(-1, "0x00000000"), std::invalid_argument);
}

TEST_F(SSDTest, Read_upper_lba_fail_test) {
	EXPECT_CALL(m_nand_, write(101, "0x00000000"))
		.Times(0);

	EXPECT_THROW(ssd_.write(101, "0x00000000"), std::invalid_argument);
}

TEST_F(SSDTest, Read_data_out_of_rang_fail_test) {
	EXPECT_CALL(m_nand_, write(0, "0x0000000000"))
		.Times(0);

	EXPECT_THROW(ssd_.write(0, "0x0000000000"), std::invalid_argument);
}

TEST_F(SSDTest, Read_data_prefix_fail_test) {
	EXPECT_CALL(m_nand_, write(0, "xx00000000"))
		.Times(0);

	EXPECT_THROW(ssd_.write(0, "xx00000000"), std::invalid_argument);
}

TEST_F(SSDTest, Write_test) {
	// TODO
}

// TODO