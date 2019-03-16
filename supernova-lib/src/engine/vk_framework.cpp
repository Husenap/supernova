#include "vk_framework.h"

#include "../precompiled.h"

#include "window.h"

const std::vector<const char*> device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const std::vector<const char*> validation_layers = {"VK_LAYER_LUNARG_standard_validation"};

#ifndef NDEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace snova {
vk_framework::vk_framework()
	: m_physical_device(VK_NULL_HANDLE) {}

vk_framework::~vk_framework() {}

bool vk_framework::init() {
	if (!create_vk_instance()) return false;
	if (enable_validation_layers && !setup_debug_messenger()) return false;
	if (!create_surface()) return false;
	if (!pick_physical_device()) return false;
	if (!create_logical_device()) return false;
	if (!create_swapchain()) return false;
	if (!create_image_views()) return false;
	if (!create_render_pass()) return false;
	if (!create_graphics_pipeline()) return false;
	if (!create_frame_buffers()) return false;
	if (!create_command_pool()) return false;
	if (!create_command_buffers()) return false;
	if (!create_sync_objects()) return false;

	window::register_resize_callback([& flag = m_framebuffer_resized](auto) { flag = true; });

	VERBOSE_LOG("Created vulkan framework");
	return true;
}

bool vk_framework::create_vk_instance() {
	if (enable_validation_layers && !check_validation_layer_support()) {
		ERROR_LOG("Validation layers requested but not available!");
		return false;
	}

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Herro Triangor";
	app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.pEngineName = "SuperNova";
	app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	auto required_extensions = get_requried_extensions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
	create_info.ppEnabledExtensionNames = required_extensions.data();

	if (enable_validation_layers) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	} else {
		create_info.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&create_info, nullptr, &m_vk_instance) != VK_SUCCESS) {
		FATAL_LOG("vkCreateInstance returned error code!");
		return false;
	}

	VERBOSE_LOG("Created vulkan instance");
	return true;
}

VkResult create_debug_utils_messnger_ext(VkInstance instance,
										 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
										 const VkAllocationCallbacks* pAllocator,
										 VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
void destroy_debug_utils_messenger_ext(VkInstance instance,
									   VkDebugUtilsMessengerEXT debugMessenger,
									   const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
																		   "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

bool vk_framework::setup_debug_messenger() {
	VkDebugUtilsMessengerCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = vlayer_callback;
	create_info.pUserData = nullptr;

	if (create_debug_utils_messnger_ext(m_vk_instance, &create_info, nullptr, &m_debug_messenger) !=
		VK_SUCCESS) {
		ERROR_LOG("Failed to create debug utils messenger");
		return false;
	}

	VERBOSE_LOG("Set Up Debug Messenger");
	return true;
}

bool vk_framework::create_surface() {
	if (glfwCreateWindowSurface(m_vk_instance, window::get_window_handle(), nullptr, &m_surface) !=
		VK_SUCCESS) {
		FATAL_LOG("Failed to create window surface");
		return false;
	}

	VERBOSE_LOG("Created surface");
	return true;
}

bool vk_framework::pick_physical_device() {
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_vk_instance, &device_count, nullptr);

	if (device_count == 0) {
		FATAL_LOG("Failed to find any GPUs with Vulkan support!");
		return false;
	}

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(m_vk_instance, &device_count, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		candidates.insert(std::make_pair(rate_device_suitability(device), device));
	}

	if (candidates.rbegin()->first > 0) {
		m_physical_device = candidates.rbegin()->second;
	} else {
		FATAL_LOG("Failed to find a suitable GPU!");
		return false;
	}

	VkPhysicalDeviceProperties device_props;
	vkGetPhysicalDeviceProperties(m_physical_device, &device_props);

	INFO_LOG("Picked physical device: %s", device_props.deviceName);
	return true;
}

int vk_framework::rate_device_suitability(VkPhysicalDevice device) {
	if (!check_device_extension_support(device)) {
		return 0;
	}

	auto swapchain_support = query_swapchain_support(device);
	if (swapchain_support.surface_formats.empty() || swapchain_support.present_modes.empty()) {
		return 0;
	}

	if (!find_queue_families(device).is_complete()) {
		return 0;
	}

	VkPhysicalDeviceProperties device_props;
	vkGetPhysicalDeviceProperties(device, &device_props);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

	if (!device_features.geometryShader) {
		return 0;
	}

	int score = 0;
	score += 1000 * (device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
	score += device_props.limits.maxImageDimension2D;

	return score;
}

bool vk_framework::check_device_extension_support(VkPhysicalDevice device) {
	uint32_t extension_count = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

	for (const auto& extension : available_extensions) {
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}

queue_family_indices vk_framework::find_queue_families(VkPhysicalDevice device) {
	queue_family_indices indices;

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	for (uint32_t i = 0; !indices.is_complete() && i < queue_family_count; ++i) {
		const auto& queue_family = queue_families[i];

		if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.m_graphics_family = i;
		}

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &present_support);
		if (queue_family.queueCount > 0 && present_support) {
			indices.m_present_family = i;
		}
	}

	return indices;
}

bool vk_framework::create_logical_device() {
	queue_family_indices indices = find_queue_families(m_physical_device);

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = {indices.m_graphics_family.value(),
												indices.m_present_family.value()};

	float queue_priority = 1.0f;
	for (uint32_t queue_family : unique_queue_families) {
		VkDeviceQueueCreateInfo queue_create_info = {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		queue_create_infos.push_back(queue_create_info);
	}

	VkPhysicalDeviceFeatures device_features = {};

	VkDeviceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	create_info.pEnabledFeatures = &device_features;

	create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
	create_info.ppEnabledExtensionNames = device_extensions.data();

	if (enable_validation_layers) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	} else {
		create_info.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device) != VK_SUCCESS) {
		FATAL_LOG("Failed to create logical device!");
		return false;
	}

	vkGetDeviceQueue(m_device, indices.m_graphics_family.value(), 0, &m_graphics_queue);
	vkGetDeviceQueue(m_device, indices.m_present_family.value(), 0, &m_present_queue);

	VERBOSE_LOG("Created logical device");
	return true;
}

bool vk_framework::create_swapchain() {
	auto details = query_swapchain_support(m_physical_device);

	auto extent = choose_swap_extent(details.capabilities);
	auto surface_format = choose_swap_surface_format(details.surface_formats);
	auto present_mode = choose_swap_present_mode(details.present_modes);

	uint32_t image_count = details.capabilities.minImageCount + 1;
	if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
		image_count = details.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = m_surface;

	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	queue_family_indices indices = find_queue_families(m_physical_device);
	uint32_t unique_queue_families[] = {indices.m_graphics_family.value(), indices.m_present_family.value()};

	if (indices.m_graphics_family != indices.m_present_family) {
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = unique_queue_families;
	} else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = nullptr;
	}

	create_info.preTransform = details.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain) != VK_SUCCESS) {
		FATAL_LOG("Failed to create the swapchain!");
		return false;
	}

	vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
	m_swapchain_images.resize(image_count);
	vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

	m_swapchain_image_format = surface_format.format;
	m_swapchain_extent = extent;

	VERBOSE_LOG("Created swapchain");
	return true;
}

snova::swapchain_support_details vk_framework::query_swapchain_support(VkPhysicalDevice device) {
	swapchain_support_details details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, nullptr);
	if (format_count != 0) {
		details.surface_formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device, m_surface, &format_count, details.surface_formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, nullptr);
	if (present_mode_count != 0) {
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, m_surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}

VkSurfaceFormatKHR vk_framework::choose_swap_surface_format(
	const std::vector<VkSurfaceFormatKHR>& available_formats) {
	if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
		return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}

	for (const auto& format : available_formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
			format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}

	return available_formats[0];
}

VkPresentModeKHR vk_framework::choose_swap_present_mode(
	const std::vector<VkPresentModeKHR>& available_present_modes) {
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& present_mode : available_present_modes) {
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return present_mode;
		} else if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			best_mode = present_mode;
		}
	}

	return best_mode;
}

VkExtent2D vk_framework::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		auto size = window::get_window_size();
		VkExtent2D actual_extent = {static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)};

		actual_extent.width = std::max(capabilities.minImageExtent.width,
									   std::min(capabilities.maxImageExtent.width, actual_extent.width));
		actual_extent.height = std::max(capabilities.minImageExtent.height,
										std::min(capabilities.maxImageExtent.height, actual_extent.height));

		return actual_extent;
	}
}

bool vk_framework::create_image_views() {
	m_swapchain_image_views.resize(m_swapchain_images.size());

	for (size_t i = 0; i < m_swapchain_images.size(); ++i) {
		VkImageViewCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image = m_swapchain_images[i];
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = m_swapchain_image_format;

		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_device, &create_info, nullptr, &m_swapchain_image_views[i]) != VK_SUCCESS) {
			FATAL_LOG("Failed to create image view!");
		}
	}

	VERBOSE_LOG("Created image views");
	return true;
}

bool vk_framework::create_render_pass() {
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = m_swapchain_image_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	if (vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
		FATAL_LOG("Failed to create render pass!");
		return false;
	}

	VERBOSE_LOG("Created render pass");
	return true;
}

static std::vector<char> read_file(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		FATAL_LOG("Failed to open file");
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);

	file.close();

	return buffer;
}
bool vk_framework::create_graphics_pipeline() {
	auto vert_shader_code = read_file("assets/shaders/simple.vert.spv");
	auto frag_shader_code = read_file("assets/shaders/simple.frag.spv");

	VkShaderModule vert_shader_module = create_shader_module(vert_shader_code);
	VkShaderModule frag_shader_module = create_shader_module(frag_shader_code);

	VkPipelineShaderStageCreateInfo vert_shader_stage_info = {};
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_shader_stage_info.module = vert_shader_module;
	vert_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo frag_shader_stage_info = {};
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.module = frag_shader_module;
	frag_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

	VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
	vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_state.vertexBindingDescriptionCount = 0;
	vertex_input_state.pVertexBindingDescriptions = nullptr;
	vertex_input_state.vertexAttributeDescriptionCount = 0;
	vertex_input_state.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
	input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly_state.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapchain_extent.width;
	viewport.height = (float)m_swapchain_extent.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = m_swapchain_extent;

	VkPipelineViewportStateCreateInfo viewport_state = {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState color_blend_attachment = {};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
											VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

	VkPipelineDynamicStateCreateInfo dynamic_state = {};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	if (vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
		FATAL_LOG("Failed to create pipeline layout!");
		return false;
	}

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vertex_input_state;
	pipeline_info.pInputAssemblyState = &input_assembly_state;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.layout = m_pipeline_layout;
	pipeline_info.renderPass = m_render_pass;
	pipeline_info.subpass = 0;

	if (vkCreateGraphicsPipelines(
			m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline)) {
		FATAL_LOG("Failed to create graphics pipeline!");
		return false;
	}

	vkDestroyShaderModule(m_device, vert_shader_module, nullptr);
	vkDestroyShaderModule(m_device, frag_shader_module, nullptr);

	VERBOSE_LOG("Created graphics pipeline");
	return true;
}

VkShaderModule vk_framework::create_shader_module(const std::vector<char>& code) {
	VkShaderModuleCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code.size();
	create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shader_module;
	if (vkCreateShaderModule(m_device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
		FATAL_LOG("Failed to create shader module");
	}

	return shader_module;
}

bool vk_framework::create_frame_buffers() {
	m_swapchain_framebuffers.resize(m_swapchain_image_views.size());

	for (size_t i = 0; i < m_swapchain_image_views.size(); ++i) {
		VkImageView attachments[] = {m_swapchain_image_views[i]};

		VkFramebufferCreateInfo framebuffer_info = {};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = m_render_pass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = attachments;
		framebuffer_info.width = m_swapchain_extent.width;
		framebuffer_info.height = m_swapchain_extent.height;
		framebuffer_info.layers = 1;

		if (vkCreateFramebuffer(m_device, &framebuffer_info, nullptr, &m_swapchain_framebuffers[i]) !=
			VK_SUCCESS) {
			FATAL_LOG("Failed to create framebuffer!");
			return false;
		}
	}

	VERBOSE_LOG("Created framebuffers");
	return true;
}

bool vk_framework::create_command_pool() {
	queue_family_indices family_indices = find_queue_families(m_physical_device);

	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = family_indices.m_graphics_family.value();

	if (vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool) != VK_SUCCESS) {
		FATAL_LOG("Failed to create command pool!");
		return false;
	}

	VERBOSE_LOG("created command pool");
	return true;
}

bool vk_framework::create_command_buffers() {
	m_command_buffers.resize(m_swapchain_framebuffers.size());

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = m_command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = (uint32_t)m_command_buffers.size();

	if (vkAllocateCommandBuffers(m_device, &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
		FATAL_LOG("Failed to allocate command buffers!");
		return false;
	}

	for (size_t i = 0; i < m_command_buffers.size(); ++i) {
		auto& command_buffer = m_command_buffers[i];

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
			FATAL_LOG("Failed to begin recording command buffer!");
			return false;
		}

		VkRenderPassBeginInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = m_render_pass;
		render_pass_info.framebuffer = m_swapchain_framebuffers[i];
		render_pass_info.renderArea.offset = {0, 0};
		render_pass_info.renderArea.extent = m_swapchain_extent;
		VkClearValue clear_color = {0.f, 0.f, 0.f, 1.f};
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = &clear_color;

		vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

		vkCmdDraw(command_buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(command_buffer);

		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
			FATAL_LOG("Failed to record command buffer!");
			return false;
		}
	}

	VERBOSE_LOG("created command buffers");
	return true;
}

bool vk_framework::create_sync_objects() {
	m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available_semaphores[i]) !=
				VK_SUCCESS ||
			vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_finished_semaphores[i]) !=
				VK_SUCCESS ||
			vkCreateFence(m_device, &fence_info, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS) {
			FATAL_LOG("Failed to create sync objects for a frame!");
			return false;
		}
	}

	VERBOSE_LOG("created sync objects");
	return true;
}

bool vk_framework::recreate_swapchain() {
	glm::ivec2 window_size(0);
	while (window_size.x == 0 || window_size.y == 0) {
		window_size = window::get_window_size();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device);

	destroy_swapchain();

	if (!create_swapchain()) return false;
	if (!create_image_views()) return false;
	if (!create_render_pass()) return false;
	if (!create_graphics_pipeline()) return false;
	if (!create_frame_buffers()) return false;
	if (!create_command_buffers()) return false;

	VERBOSE_LOG("recreated swapchain");
	return true;
}

void vk_framework::destroy() {
	vkDeviceWaitIdle(m_device);

	destroy_swapchain();

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroySemaphore(m_device, m_image_available_semaphores[i], nullptr);
		vkDestroySemaphore(m_device, m_render_finished_semaphores[i], nullptr);
		vkDestroyFence(m_device, m_in_flight_fences[i], nullptr);
	}

	vkDestroyCommandPool(m_device, m_command_pool, nullptr);

	vkDestroyDevice(m_device, nullptr);

	if (enable_validation_layers) {
		destroy_debug_utils_messenger_ext(m_vk_instance, m_debug_messenger, nullptr);
	}

	vkDestroySurfaceKHR(m_vk_instance, m_surface, nullptr);
	vkDestroyInstance(m_vk_instance, nullptr);
}

void vk_framework::draw_frame() {
	vkWaitForFences(
		m_device, 1, &m_in_flight_fences[m_current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR(m_device,
											m_swapchain,
											std::numeric_limits<uint64_t>::max(),
											m_image_available_semaphores[m_current_frame],
											VK_NULL_HANDLE,
											&image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreate_swapchain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		FATAL_LOG("Failed to acquire swap chain image!");
	}

	VkSemaphore wait_semaphores[] = {m_image_available_semaphores[m_current_frame]};
	VkSemaphore signal_semaphores[] = {m_render_finished_semaphores[m_current_frame]};
	VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_command_buffers[image_index];
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphores;

	vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame]);

	if (vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_in_flight_fences[m_current_frame]) != VK_SUCCESS) {
		FATAL_LOG("Failed to submit draw command buffer!");
	}

	VkSwapchainKHR swapchains[] = {m_swapchain};
	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapchains;
	present_info.pImageIndices = &image_index;

	result = vkQueuePresentKHR(m_graphics_queue, &present_info);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized) {
		m_framebuffer_resized = false;
		recreate_swapchain();
	} else if (result != VK_SUCCESS) {
		FATAL_LOG("Failed to present swap chain image!");
	}

	m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void vk_framework::destroy_swapchain() {
	for (auto framebuffer : m_swapchain_framebuffers) vkDestroyFramebuffer(m_device, framebuffer, nullptr);

	vkFreeCommandBuffers(
		m_device, m_command_pool, static_cast<uint32_t>(m_command_buffers.size()), m_command_buffers.data());

	vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
	vkDestroyRenderPass(m_device, m_render_pass, nullptr);

	for (auto image_view : m_swapchain_image_views) vkDestroyImageView(m_device, image_view, nullptr);

	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

bool vk_framework::check_validation_layer_support() {
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
	std::vector<VkLayerProperties> available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	for (const auto& layer_name : validation_layers) {
		for (const auto& layer_props : available_layers) {
			if (strcmp(layer_name, layer_props.layerName) == 0) {
				return true;
			}
		}
	}

	return false;
}

std::vector<const char*> vk_framework::get_requried_extensions() {
	uint32_t glfw_extension_count = 0;
	const char** glfw_extensions;

	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

	if (enable_validation_layers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_framework::vlayer_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
							  [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type,
							  [[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
							  [[maybe_unused]] void* p_user_data) {
	switch (message_severity) {
		/*
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			VERBOSE_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			INFO_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
		*/
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			WARNING_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			ERROR_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
	}

	return VK_FALSE;
}

}  // namespace snova
