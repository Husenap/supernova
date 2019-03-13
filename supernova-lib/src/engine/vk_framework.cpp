#include "vk_framework.h"

#include "../precompiled.h"

const std::vector<const char*> validation_layers = {"VK_LAYER_LUNARG_standard_validation"};

#ifndef NDEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace snova {
vk_framework::vk_framework()
	: m_physical_device(VK_NULL_HANDLE) {
}

vk_framework::~vk_framework() {
}

bool vk_framework::init() {
	if (!create_vk_instance()) {
		return false;
	}

	if (enable_validation_layers && !setup_debug_messenger()) {
		ERROR_LOG("Failed to setup debug messenger");
		return false;
	}

	if (!pick_physical_device()) {
		return false;
	}

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
	create_info.pfnUserCallback = debug_callback;
	create_info.pUserData = nullptr;

	if (create_debug_utils_messnger_ext(m_vk_instance, &create_info, nullptr, &m_debug_messenger) !=
		VK_SUCCESS) {
		ERROR_LOG("Failed to create debug utils messenger");
		return false;
	}

	VERBOSE_LOG("Set Up Debug Messenger");
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

	for (const auto& device : devices) {
		if (is_device_suitable(device)) {
			m_physical_device = device;
			break;
		}
	}

	if (m_physical_device == VK_NULL_HANDLE) {
		FATAL_LOG("Failed to find a suitable GPU!");
	}

	VERBOSE_LOG("Picked physical device");
	return true;
}

bool vk_framework::is_device_suitable(VkPhysicalDevice device) {
	

	return true;
}

void vk_framework::destroy() {
	if (enable_validation_layers) {
		destroy_debug_utils_messenger_ext(m_vk_instance, m_debug_messenger, nullptr);
	}

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
		extensions.push_back("VK_EXT_debug_utils");
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
