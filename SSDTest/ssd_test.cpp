#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../ssd/INAND.cpp"
#include "../ssd/SSD.cpp"
#include <stdexcept>

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
	EXPECT_CALL(m_nand_, write(0, "0x00000000"))
		.Times(1);
	ssd_.write(0, "0x00000000");
}

TEST_F(SSDTest, Write_lower_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(-1, "0x00000000"))
		.Times(0);

	EXPECT_NO_THROW(ssd_.write(-1, "0x00000000"));
}

TEST_F(SSDTest, Write_upper_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(101, "0x00000000"))
		.Times(0);

	EXPECT_NO_THROW(ssd_.write(101, "0x00000000"));
}

TEST_F(SSDTest, Write_data_out_of_rang_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(0, "0x0000000000"))
		.Times(0);

	EXPECT_NO_THROW(ssd_.write(0, "0x0000000000"));
}

TEST_F(SSDTest, Write_data_prefix_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(0, "xx00000000"))
		.Times(0);

	EXPECT_NO_THROW(ssd_.write(0, "xx00000000"));
}

TEST_F(SSDTest, Read_test_normal_success) {
	int testLBA1 = 0;
	EXPECT_CALL(m_nand_, read(testLBA1))
		.WillOnce(Return("0xAAAABBBB"));
	EXPECT_THAT(ssd_.read(testLBA1), Eq("0xAAAABBBB"));

	int testLBA2 = 30;
	EXPECT_CALL(m_nand_, read(testLBA2))
		.WillOnce(Return("0x0000AAAA"));
	EXPECT_THAT(ssd_.read(testLBA2), Eq("0x0000AAAA"));
}

TEST_F(SSDTest, Read_test_boundary_check_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, read(-1))
		.Times(0);

	EXPECT_THAT(ssd_.read(-1), Eq(""));

	EXPECT_CALL(m_nand_, read(100))
		.Times(0);
	EXPECT_THAT(ssd_.read(100), Eq(""));
}

TEST_F(SSDTest, Read_test_not_written_lba) {
	{
		InSequence seq;
		EXPECT_CALL(m_nand_, write(20, "0x1289CDEF"))
			.Times(1);
		EXPECT_CALL(m_nand_, read(20))
			.WillOnce(Return("0x1289CDEF"));
		EXPECT_CALL(m_nand_, read(10))
			.WillOnce(Return("0x00000000"));
	}
	ssd_.write(20, "0x1289CDEF");
	EXPECT_THAT(ssd_.read(20), Eq("0x1289CDEF"));
	EXPECT_THAT(ssd_.read(10), Eq("0x00000000"));
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
	EXPECT_CALL(m_nand_, write(0, "0x00000000"))
		.Times(1);

	int address = 0;
	std::string data = "0x00000000";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));

	invoker.executeCommand();
}

TEST_F(SSDCommandInvokerTest, Write_lower_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(-1, "0x00000000"))
		.Times(0);

	int address = -1;
	std::string data = "0x00000000";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_upper_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(101, "0x00000000"))
		.Times(0);

	int address = 101;
	std::string data = "0x00000000";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_data_out_of_rang_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(0, "0x0000000000"))
		.Times(0);

	int address = 0;
	std::string data = "0x0000000000";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Write_data_prefix_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, write(0, "xx00000000"))
		.Times(0);

	int address = 0;
	std::string data = "xx00000000";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));

	EXPECT_NO_THROW(invoker.executeCommand());
}

TEST_F(SSDCommandInvokerTest, Read_test_normal_success) {
	int testLBA1 = 0;
	EXPECT_CALL(m_nand_, read(testLBA1))
		.WillOnce(Return("0xAAAABBBB"));

	invoker.setCommand(new ReadCommand(&ssd_, testLBA1));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq("0xAAAABBBB"));

	int testLBA2 = 30;
	EXPECT_CALL(m_nand_, read(testLBA2))
		.WillOnce(Return("0x0000AAAA"));
	invoker.setCommand(new ReadCommand(&ssd_, testLBA2));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq("0x0000AAAA"));
}

TEST_F(SSDCommandInvokerTest, Read_test_boundary_check_fail_on_ssd_but_no_throw_test) {
	EXPECT_CALL(m_nand_, read(-1))
		.Times(0);

	invoker.setCommand(new ReadCommand(&ssd_, -1));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(""));

	EXPECT_CALL(m_nand_, read(100))
		.Times(0);

	invoker.setCommand(new ReadCommand(&ssd_, 100));
	invoker.executeCommand();

	EXPECT_THAT(invoker.getResult(), Eq(""));
}

TEST_F(SSDCommandInvokerTest, Read_test_not_written_lba) {
	{
		EXPECT_CALL(m_nand_, write(20, "0x1289CDEF"))
			.Times(1);
		EXPECT_CALL(m_nand_, read(20))
			.WillOnce(Return("0x1289CDEF"));
		EXPECT_CALL(m_nand_, read(10))
			.WillOnce(Return("0x00000000"));
	}

	int address = 20;
	std::string data = "0x1289CDEF";
	invoker.setCommand(new WriteCommand(&ssd_, address, data));
	EXPECT_NO_THROW(invoker.executeCommand());

	invoker.setCommand(new ReadCommand(&ssd_, 20));
	invoker.executeCommand();
	EXPECT_THAT(invoker.getResult(), Eq("0x1289CDEF"));

	invoker.setCommand(new ReadCommand(&ssd_, 10));
	invoker.executeCommand();
	EXPECT_THAT(invoker.getResult(), Eq("0x00000000"));
}