#include "vk_framework.h"

#include "../precompiled.h"

const std::vector<const char*> validation_layers = {"VK_LAYER_LUNARG_standard_validation"};

#ifndef NDEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace snova {
vk_framework::vk_framework() {
}

vk_framework::~vk_framework() {
}

bool vk_framework::init() {
	if (enable_validation_layers && !check_validation_layer_support()) {
		ERROR_LOG("Validation layers requested but not available!");
		return false;
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Herro Triangor";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "SuperNova";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto required_extensions = get_requried_extensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
	createInfo.ppEnabledExtensionNames = required_extensions.data();

	if (enable_validation_layers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		createInfo.ppEnabledLayerNames = validation_layers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &m_vk_instance) != VK_SUCCESS) {
		FATAL_LOG("FAILED TO CREATE VULKAN INSTANCE!");
		return false;
	}

	uint32_t available_extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(available_extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, available_extensions.data());

	INFO_LOG("Available extensions:");
	for (const auto& extension : available_extensions) {
		INFO_LOG("\t* %s", extension.extensionName);
	}

	return true;
}

void vk_framework::destroy() {
	vkDestroyInstance(m_vk_instance, nullptr);
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
		extensions.push_back("VK_EXT_DEBUG_UTILS_NAME");
	}

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_framework::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
							 VkDebugUtilsMessageTypeFlagsEXT message_type,
							 const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
							 void* p_user_data) {
	switch (message_severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			VERBOSE_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			INFO_LOG("VLayer: %s", p_callback_data->pMessage);
			break;
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
