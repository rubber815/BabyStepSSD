#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../SSDProject/INAND.cpp"
#include "../SSDProject/SSD.cpp"

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

TEST_F(SSDTest, Read_test) {
	// TODO
}

TEST_F(SSDTest, Write_test_normal_success) {
	int testLBA = 0;
	EXPECT_CALL(m_nand_, read(testLBA))
		.WillOnce(Return("0xAAAABBBB"));
	EXPECT_THAT(ssd_.read(testLBA), Eq("0xAAAABBBB"));

	int testLBA = 30;
	EXPECT_CALL(m_nand_, read(testLBA))
		.WillOnce(Return("0xAAAABBBB"));
	EXPECT_THAT(ssd_.read(testLBA), Eq("0xAAAABBBB"));
}

TEST_F(SSDTest, Write_test_boundary_check_fail) {
	EXPECT_THAT(ssd_.read(-1), Eq("0x00000000")) << "Check invalid scope";
	EXPECT_THAT(ssd_.read(100), Eq("0x00000000")) << "Check invalid scope";
}

// TODO