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
	ssd_.write(99, "0x00000003");
	EXPECT_EQ("0x00000003",ssd_.read(99));
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

	//ssd_.write(30, "0x0000AAAA");
	EXPECT_EQ("0x00000000", ssd_.read(30));
}

// TODO