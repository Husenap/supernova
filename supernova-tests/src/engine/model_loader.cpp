#include "gtest/gtest.h"

#include "../precompiled.h"

TEST(model_loader, load_model_invalid_filepath){
	std::optional<snova::model_data> data = snova::model_loader::load_model("hej");

	EXPECT_EQ(data.has_value(), false);
}

TEST(model_loader, load_model_cube){
	std::optional<snova::model_data> data = snova::model_loader::load_model("res/models/cube.fbx");

	EXPECT_EQ(data.has_value(), true);

	EXPECT_EQ(data->m_vertices.size(), 24);
	EXPECT_EQ(data->m_indices.size(), 36);
}

TEST(model_loader, load_model_no_meshes){
	std::optional<snova::model_data> data = snova::model_loader::load_model("res/models/no_meshes.fbx");

	EXPECT_EQ(data.has_value(), false);
}
