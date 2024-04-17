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

TEST(NANDTest, DISABLED_Nand_single_write_read_test) {
	BabyStepNand bsn;
	bsn.write(0x3, "0x444");
	EXPECT_EQ("0x444", bsn.read(0x3));
}

TEST_F(SSDTestWithBabyStepNand, DISABLED_Write_lba_test_with_BabyStep) {
	ssd_.write(0x4, "0x00000003");
	EXPECT_EQ("0x00000003",ssd_.read(0x4));
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


// TODO