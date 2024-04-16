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
	ssd_.write(0x4, "0x00000003");
	EXPECT_EQ("0x00000003",ssd_.read(0x4));
}

// TODO