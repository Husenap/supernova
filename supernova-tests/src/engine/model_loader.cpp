#include "gtest/gtest.h"

#include "../precompiled.h"

TEST(model_loader, load_model){
	std::optional<snova::model_data> data = snova::model_loader::load_model("hej");

	EXPECT_EQ(data.has_value(), false);
}
