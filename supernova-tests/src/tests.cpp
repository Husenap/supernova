#include "gtest/gtest.h"

TEST(gtest, test) {
	int i = 1;
	EXPECT_TRUE(i == 1);
	EXPECT_FALSE(i != 1);
}