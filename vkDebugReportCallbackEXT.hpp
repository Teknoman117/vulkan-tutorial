#ifndef __VK_DEBUG_REPORT_CALLBACK_EXT_HPP__
#define __VK_DEBUG_REPORT_CALLBACK_EXT_HPP__

#include <vulkan/vulkan.h>

// Create debug report callback extension
VkResult CreateDebugReportCallbackEXT(VkInstance instance, 
                                      const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugReportCallbackEXT* pCallback) 
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(VkInstance instance, 
								   VkDebugReportCallbackEXT callback,
								   const VkAllocationCallbacks* pAllocator) 
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
    	func(instance, callback, pAllocator);
	}
}

#endif
