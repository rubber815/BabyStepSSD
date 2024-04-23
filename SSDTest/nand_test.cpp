#pragma once

#include <gtest/gtest.h>

#include <stdexcept>
#include "test.hpp"

using namespace testing;

class SSDTestWithBabyStepNand : public testing::Test {
protected:
	void SetUp() override {
		std::remove("nand.txt");
		ssd_.selectNAND(&b_nand_);

		for (int i = 0; i < 100; i++)
			b_nand_.write(i, NORMAL_VALUE_ZERO);
	}
public:
	SSD ssd_;
	BabyStepNand b_nand_;
};

TEST(NANDTest, Nand_single_write_read_test) {
	std::remove("nand.txt");

	BabyStepNand bsn;

	bsn.write(NORMAL_LBA_NON_ZERO, NORMAL_VALUE_AAAABBBB);
	EXPECT_EQ(NORMAL_VALUE_AAAABBBB, bsn.read(NORMAL_LBA_NON_ZERO));
}

TEST_F(SSDTestWithBabyStepNand, Write_lba_test_with_BabyStep) {

	ssd_.write(NORMAL_LBA_NON_ZERO, NORMAL_VALUE_0000AAAA);
	EXPECT_EQ(NORMAL_VALUE_0000AAAA, ssd_.read(NORMAL_LBA_NON_ZERO));
}

TEST_F(SSDTestWithBabyStepNand, Full_Read_test_with_BabyStep_Normal_Success) {
	for (int lba = 0; lba < 100; lba++)
		ssd_.write(lba, NORMAL_VALUE_AAAABBBB);

	for (int lba = 0; lba < 100; lba++)
		EXPECT_EQ(ssd_.read(lba), NORMAL_VALUE_AAAABBBB);
}
	

TEST_F(SSDTestWithBabyStepNand, Full_Read_test_with_BabyStep_Normal_Fail) {
	for (int lba = 0; lba < 50; lba++)
		ssd_.write(lba, NORMAL_VALUE_AAAABBBB);

	for (int lba = 50; lba < 100; lba++)
		ssd_.write(lba, NORMAL_VALUE_0000AAAA);

	for (int lba = 0; lba < 50; lba++)
		EXPECT_EQ(ssd_.read(lba), NORMAL_VALUE_AAAABBBB);

	for (int lba = 50; lba < 100; lba++)
		EXPECT_NE(ssd_.read(lba), NORMAL_VALUE_AAAABBBB);
}

TEST_F(SSDTestWithBabyStepNand, Write_lower_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_NO_THROW(ssd_.write(INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_AAAABBBB));
}

TEST_F(SSDTestWithBabyStepNand, Write_upper_lba_fail_on_ssd_but_no_throw_test) {
	EXPECT_NO_THROW(ssd_.write(INVALID_LBA_UNDER_RANGE, NORMAL_VALUE_AAAABBBB));
}

TEST_F(SSDTestWithBabyStepNand, Write_data_out_of_range_fail_on_ssd_but_no_throw_test) {
	EXPECT_NO_THROW(ssd_.write(NORMAL_LBA_NON_ZERO, INVALID_VALUE_ZERO_OVER_RANGE));
}

TEST_F(SSDTestWithBabyStepNand, Write_data_prefix_fail_on_ssd_but_no_throw_test) {
	EXPECT_NO_THROW(ssd_.write(NORMAL_LBA_NON_ZERO, INVALID_VALUE_WRONG_PREFIX));
}

TEST_F(SSDTestWithBabyStepNand, Write_Read_test_normal_success) {
	ssd_.write(NORMAL_LBA_ZERO, NORMAL_VALUE_AAAABBBB);
	EXPECT_EQ(NORMAL_VALUE_AAAABBBB, ssd_.read(NORMAL_LBA_ZERO));

	ssd_.write(NORMAL_LBA_10, NORMAL_VALUE_AAAABBBB);
	EXPECT_EQ(NORMAL_VALUE_AAAABBBB, ssd_.read(NORMAL_LBA_10));
}

TEST_F(SSDTestWithBabyStepNand, Write_Read_test_not_written_lba) {
	ssd_.write(NORMAL_LBA_ZERO, NORMAL_VALUE_AAAABBBB);
	EXPECT_EQ(NORMAL_VALUE_AAAABBBB, ssd_.read(NORMAL_LBA_ZERO));
	EXPECT_EQ(NORMAL_VALUE_ZERO, ssd_.read(NORMAL_LBA_30));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_success_test_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		ssd_.write(i, NORMAL_VALUE_AAAABBBB);

	for (int i = 0; i < 100; i++)
		EXPECT_EQ(NORMAL_VALUE_AAAABBBB, ssd_.read(i));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_on_ssd_but_no_throw_test_lba1_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_NO_THROW(ssd_.write(i - 100, NORMAL_VALUE_AAAABBBB));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_on_ssd_but_no_throw_test_lba2_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_NO_THROW(ssd_.write(i + 100, NORMAL_VALUE_AAAABBBB));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_on_ssd_but_no_throw_test_value1_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_NO_THROW(ssd_.write(i, INVALID_VALUE_WRONG_PREFIX));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_fail_on_ssd_but_no_throw_test_value2_with_BabyStep) {
	for (int i = 0; i < 100; i++)
		EXPECT_NO_THROW(ssd_.write(i, INVALID_VALUE_ZERO_OVER_RANGE));
}

TEST_F(SSDTestWithBabyStepNand, fullWrite_twice_success_test_with_BabyStep) {
	for (int i = 0; i < 100; i++) {
		ssd_.write(i, NORMAL_VALUE_AAAABBBB);
		ssd_.write(i, NORMAL_VALUE_0000AAAA);
	}

	for (int i = 0; i < 100; i++)
		EXPECT_EQ(NORMAL_VALUE_0000AAAA, ssd_.read(i));
}
