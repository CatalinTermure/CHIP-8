#ifndef CHIP8_SRC_DEBUG_MESSENGER_H_
#define CHIP8_SRC_DEBUG_MESSENGER_H_
#include <vulkan/vulkan.hpp>
#include <fstream>
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													 VkDebugUtilsMessageTypeFlagsEXT messageTypes,
													 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
													 void *pUserData) {
  if (messageSeverity==VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
	std::cout << "Validation layers ERROR: " << pCallbackData->pMessage << '\n';
	assert(!"Validation layers threw an error");
  } else if (messageSeverity==VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
	std::cout << "Validation layers WARNING: " << pCallbackData->pMessage << '\n';
	assert(!"Validation layers gave a warning");
  } else if(messageSeverity==VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
	std::cout << "Validation layers VERBOSE: " << pCallbackData->pMessage << '\n';
  }

  return VK_FALSE;
}

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
															  const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
															  const VkAllocationCallbacks *pAllocator,
															  VkDebugUtilsMessengerEXT *pMessenger) {
  return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
														   VkDebugUtilsMessengerEXT messenger,
														   VkAllocationCallbacks const *pAllocator) {
  return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

void SetupDebugMessenger(vk::Instance instance) {
  pfnVkCreateDebugUtilsMessengerEXT =
	  reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
  if (!pfnVkCreateDebugUtilsMessengerEXT) {
	std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
	exit(1);
  }

  pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
	  instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
  if (!pfnVkDestroyDebugUtilsMessengerEXT) {
	std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
	exit(1);
  }

  vk::DebugUtilsMessageSeverityFlagsEXT severity_flags =
	  vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
		  vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;

  vk::DebugUtilsMessageTypeFlagsEXT message_type_flags =
	  vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		  vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		  vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

  vk::DebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{
	  {}, severity_flags, message_type_flags, &DebugMessenger
  };

  instance.createDebugUtilsMessengerEXT(debug_messenger_create_info);
}

#endif //CHIP8_SRC_DEBUG_MESSENGER_H_