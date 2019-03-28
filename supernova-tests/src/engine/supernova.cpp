#include "gtest/gtest.h"

#include "../precompiled.h"

TEST(supernova, constructor){
	auto supernova = std::make_unique<snova::supernova>();

	EXPECT_NE(supernova, nullptr);
}
