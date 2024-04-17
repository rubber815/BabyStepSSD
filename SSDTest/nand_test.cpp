#pragma once

#include <gtest/gtest.h>

#include <stdexcept>
#include "../SSDProject/BabyStepNAND.cpp"
#include "../SSDProject/SSD.cpp"

using namespace testing;

class SSDTestWithBabyStepNand : public testing::Test {
protected:
	void SetUp() override {
		ssd_.selectNAND(&b_nand_);

		for (int i = 0; i < 100; i++)
			b_nand_.write(i, "0x00000000");
	}
public:
	SSD ssd_;
	BabyStepNand b_nand_;
};

TEST(NANDTest, Nand_single_write_read_test) {
	BabyStepNand bsn;
	bsn.write(0x3, "0x444");
	EXPECT_EQ("0x444", bsn.read(0x3));
}

TEST_F(SSDTestWithBabyStepNand, Write_lba_test_with_BabyStep) {
	ssd_.write(0x4, "0x00000003");
	EXPECT_EQ("0x00000003",ssd_.read(0x4));
}

TEST_F(SSDTestWithBabyStepNand, Full_Read_test_with_BabyStep_Normal_Success) {
	const std::string input = "0xABCDEFAB";
	for (int lba = 0; lba < 100; lba++) {
		ssd_.write(lba, input);
	}

	for (int lba = 0; lba < 100; lba++) {
		std::string ret = ssd_.read(lba);
		EXPECT_EQ(ret, input);
	}
}
	

TEST_F(SSDTestWithBabyStepNand, Full_Read_test_with_BabyStep_Normal_Fail) {
	const std::string input1 = "0xABCDEFAB";
	for (int lba = 0; lba < 50; lba++) {
		ssd_.write(lba, input1);
	}

	const std::string input2 = "0xABCDAAAA";
	for (int lba = 50; lba < 100; lba++) {
		ssd_.write(lba, input2);
	}

	for (int lba = 0; lba < 50; lba++) {
		std::string ret = ssd_.read(lba);
		EXPECT_EQ(ret, input1);
	}

	for (int lba = 50; lba < 100; lba++) {
		std::string ret = ssd_.read(lba);
		EXPECT_NE(ret, input1);
	}
}

TEST_F(SSDTestWithBabyStepNand, Write_lower_lba_fail_test) {
	EXPECT_THROW(ssd_.write(-1, "0x00000003"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, Write_upper_lba_fail_test) {
	EXPECT_THROW(ssd_.write(101, "0x00000003"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, Write_data_out_of_range_fail_test) {
	EXPECT_THROW(ssd_.write(3, "0x0000000003"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, Write_data_prefix_fail_test) {
	EXPECT_THROW(ssd_.write(3, "xx00000003"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, Write_Read_test_normal_success) {
	ssd_.write(0, "0xAAAABBBB");
	EXPECT_EQ("0xAAAABBBB", ssd_.read(0));

	ssd_.write(30, "0x0000AAAA");
	EXPECT_EQ("0x0000AAAA", ssd_.read(30));
}

TEST_F(SSDTestWithBabyStepNand, Write_Read_test_not_written_lba) {
	ssd_.write(0, "0xAAAABBBB");
	EXPECT_EQ("0xAAAABBBB", ssd_.read(0));

	// ssd_.write(30, "0x0000AAAA");
	EXPECT_EQ("0x00000000", ssd_.read(30));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_success_test_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		ssd_.write(i, "0xABCDFFFF");

	for (int i = 0; i < 100; i++)
		EXPECT_EQ("0xABCDFFFF", ssd_.read(i));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_test_lba1_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_THROW(ssd_.write(i-100, "0xABCDFFFF"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_test_lba2_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_THROW(ssd_.write(i + 100, "0xABCDFFFF"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_test_value1_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_THROW(ssd_.write(i, "xxABCDFFFF"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail2_test_value2_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_THROW(ssd_.write(i, "0xABCDFFFFFF"), std::invalid_argument);
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_twice_success_test_with_BabyStep) {
	for (int i = 0; i < 100; i++) {
		ssd_.write(i, "0xABCDFFFF");
		ssd_.write(i, "0x99999999");
	}

	for (int i = 0; i < 100; i++)
		EXPECT_EQ("0x99999999", ssd_.read(i));
}