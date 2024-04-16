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
	MOCK_METHOD(void, write, (int, int), (override));
	MOCK_METHOD(int, read, (int), (override));
};

TEST(TestCaseName, TestName) {
	MockNAND n;
}