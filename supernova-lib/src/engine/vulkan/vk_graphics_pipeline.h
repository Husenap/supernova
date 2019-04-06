#pragma once

#include "../../precompiled.h"

#include "vk_shader.h"

namespace snova {
struct graphics_pipeline_create_info {
	std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages;
	VkExtent2D m_extent;
	VkDescriptorSetLayout m_descriptor_set_layout;
	VkRenderPass m_render_pass;
};

class vk_graphics_pipeline {
public:
	bool create_pipeline(const graphics_pipeline_create_info& create_info);
	void destroy();

	const VkPipeline& get_pipeline() const { return m_pipeline; }
	const VkPipelineLayout& get_pipeline_layout() const { return m_pipeline_layout; }

private:
	VkPipeline m_pipeline;
	VkPipelineLayout m_pipeline_layout;

};
}  // namespace snova
