#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../ssd/INAND.cpp"
#include "../ssd/SSD.cpp"
#include <stdexcept>
#include "test.hpp"

using namespace testing;

class MockNAND : public INAND {
public:
	MOCK_METHOD(void, erase, (int, int), (override));
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

TEST_F(SSDTest, Write_lba_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ONE);
	ssd_.write(NORMAL_LBA_ZERO, NORMAL_VALUE_ZERO);
}

TEST_F(SSDTest, Write_lower_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ZERO);

	EXPECT_NO_THROW(ssd_.write(INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_ZERO));
}

TEST_F(SSDTest, Write_upper_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(INVALID_LBA_OVER_RANGE, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ZERO);

	EXPECT_NO_THROW(ssd_.write(INVALID_LBA_OVER_RANGE, NORMAL_VALUE_ZERO));
}

TEST_F(SSDTest, Write_data_out_of_rang_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, INVALID_VALUE_ZERO_OVER_RANGE))
		.Times(CALLED_TIME_ZERO);

	EXPECT_NO_THROW(ssd_.write(NORMAL_LBA_ZERO, INVALID_VALUE_ZERO_OVER_RANGE));
}

TEST_F(SSDTest, Write_data_prefix_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, INVALID_VALUE_WRONG_PREFIX))
		.Times(CALLED_TIME_ZERO);

	EXPECT_NO_THROW(ssd_.write(NORMAL_LBA_ZERO, INVALID_VALUE_WRONG_PREFIX));
}

TEST_F(SSDTest, Read_test_normal_success) {
	EXPECT_CALL(m_nand_, read(NORMAL_LBA_ZERO))
		.WillOnce(Return(NORMAL_VALUE_AAAABBBB));
	EXPECT_THAT(ssd_.read(NORMAL_LBA_ZERO), Eq(NORMAL_VALUE_AAAABBBB));

	EXPECT_CALL(m_nand_, read(NORMAL_LBA_10))
		.WillOnce(Return(NORMAL_VALUE_0000AAAA));
	EXPECT_THAT(ssd_.read(NORMAL_LBA_10), Eq(NORMAL_VALUE_0000AAAA));
}

TEST_F(SSDTest, Read_test_boundary_check_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, read(INVALID_LBA_UNDER_RANGE))
		.Times(CALLED_TIME_ZERO);

	EXPECT_THAT(ssd_.read(INVALID_LBA_UNDER_RANGE), Eq(EMPTY_STRING));

	EXPECT_CALL(m_nand_, read(INVALID_LBA_OVER_RANGE))
		.Times(CALLED_TIME_ZERO);
	EXPECT_THAT(ssd_.read(INVALID_LBA_OVER_RANGE), Eq(EMPTY_STRING));
}

TEST_F(SSDTest, Read_test_not_written_lba) {
	{
		InSequence seq;
		EXPECT_CALL(m_nand_, write(NORMAL_LBA_30, NORMAL_VALUE_1289CDEF))
			.Times(CALLED_TIME_ONE);
		EXPECT_CALL(m_nand_, read(NORMAL_LBA_30))
			.WillOnce(Return(NORMAL_VALUE_1289CDEF));
		EXPECT_CALL(m_nand_, read(NORMAL_LBA_10))
			.WillOnce(Return(NORMAL_VALUE_ZERO));
	}
	ssd_.write(NORMAL_LBA_30, NORMAL_VALUE_1289CDEF);
	EXPECT_THAT(ssd_.read(NORMAL_LBA_30), Eq(NORMAL_VALUE_1289CDEF));
	EXPECT_THAT(ssd_.read(NORMAL_LBA_10), Eq(NORMAL_VALUE_ZERO));
}

class SSDCommandInvokerTest : public testing::Test {
protected:
	void SetUp() override {
		ssd_.selectNAND(&m_nand_);
	}
public:
	SSD ssd_;
	MockNAND m_nand_;
	SSDInvoker invoker;
};

TEST_F(SSDCommandInvokerTest, Write_lba_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ONE);

	invoker.setCommand(new WriteCommand(&ssd_, NORMAL_LBA_ZERO, NORMAL_VALUE_ZERO));

	invoker.executeCommand();
}

TEST_F(SSDCommandInvokerTest, Write_lower_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new WriteCommand(&ssd_, INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_ZERO));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_upper_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(INVALID_LBA_OVER_RANGE, NORMAL_VALUE_ZERO))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new WriteCommand(&ssd_, INVALID_LBA_OVER_RANGE, NORMAL_VALUE_ZERO));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_data_out_of_rang_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, INVALID_VALUE_ZERO_OVER_RANGE))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new WriteCommand(&ssd_, NORMAL_LBA_ZERO, INVALID_VALUE_ZERO_OVER_RANGE));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_data_prefix_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(NORMAL_LBA_ZERO, INVALID_VALUE_WRONG_PREFIX))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new WriteCommand(&ssd_, NORMAL_LBA_ZERO, INVALID_VALUE_WRONG_PREFIX));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Read_test_normal_success) {
	EXPECT_CALL(m_nand_, read(NORMAL_LBA_ZERO))
		.WillOnce(Return(NORMAL_VALUE_AAAABBBB));

	invoker.setCommand(new ReadCommand(&ssd_, NORMAL_LBA_ZERO));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(NORMAL_VALUE_AAAABBBB));

	EXPECT_CALL(m_nand_, read(NORMAL_LBA_30))
		.WillOnce(Return(NORMAL_VALUE_0000AAAA));
	invoker.setCommand(new ReadCommand(&ssd_, NORMAL_LBA_30));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(NORMAL_VALUE_0000AAAA));
}

TEST_F(SSDCommandInvokerTest, Read_test_boundary_check_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, read(INVALID_LBA_UNDER_RANGE))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new ReadCommand(&ssd_, INVALID_LBA_UNDER_RANGE));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(EMPTY_STRING));

	EXPECT_CALL(m_nand_, read(INVALID_LBA_OVER_RANGE))
		.Times(CALLED_TIME_ZERO);

	invoker.setCommand(new ReadCommand(&ssd_, INVALID_LBA_OVER_RANGE));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(EMPTY_STRING));
}

TEST_F(SSDCommandInvokerTest, Read_test_not_written_lba) {
	{
		EXPECT_CALL(m_nand_, write(NORMAL_LBA_NON_ZERO, NORMAL_VALUE_1289CDEF))
			.Times(CALLED_TIME_ONE);
		EXPECT_CALL(m_nand_, read(NORMAL_LBA_NON_ZERO))
			.WillOnce(Return(NORMAL_VALUE_1289CDEF));
		EXPECT_CALL(m_nand_, read(NORMAL_LBA_10))
			.WillOnce(Return(NORMAL_VALUE_ZERO));
	}

	invoker.setCommand(new WriteCommand(&ssd_, NORMAL_LBA_NON_ZERO, NORMAL_VALUE_1289CDEF));
	EXPECT_NO_THROW(invoker.executeCommand());

	invoker.setCommand(new ReadCommand(&ssd_, NORMAL_LBA_NON_ZERO));
	invoker.executeCommand();
	EXPECT_THAT(invoker.getResult(), Eq(NORMAL_VALUE_1289CDEF));

	invoker.setCommand(new ReadCommand(&ssd_, NORMAL_LBA_10));
	invoker.executeCommand();
	EXPECT_THAT(invoker.getResult(), Eq(NORMAL_VALUE_ZERO));
}