#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <vector>

#include "vdeleter.hpp"

namespace
{
	const char *TITLE  = "01_drawtriangle";
	const int   WIDTH  = 800;
	const int   HEIGHT = 600;
}

class HelloTriangleApplication 
{
	// Open a GLFW window
	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window.reset(glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr));
	}

	void initVulkan()
	{
		createInstance();
	}

	void mainLoop()
	{
		while(!glfwWindowShouldClose(window.get())) {
			glfwPollEvents();
		}
	}

	void createInstance()
	{
		// Dump the available vulkan extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "Available Vulkan Extensions: " << std::endl;
		for(const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}
		std::cout << std::endl;

		// Get the required vulkan extensions for GLFW to function
		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::cout << "GLFW Required Vulkan Extensions: " << std::endl;
		for(size_t i = 0; i < glfwExtensionCount; i++) {
			std::cout << "\t" << glfwExtensions[i] << std::endl;
		}
		std::cout << std::endl;

		// Setup some simple application information for Vulkan
		VkApplicationInfo appInfo  = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName   = TITLE;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName        = "Derpgin";
		appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion         = VK_API_VERSION_1_0;

		// Setup the instance information
		VkInstanceCreateInfo createInfo    = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo        = &appInfo;
		createInfo.enabledExtensionCount   = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount       = 0;

		// Create the vulkan instance
		if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance");
		}
	}

public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}

private:
	VDeleter<VkInstance> instance {vkDestroyInstance};

	// Unique pointer for the window, will destroy the window on close
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window {nullptr, &glfwDestroyWindow};
};

int main(int argc, char **argv)
{
	HelloTriangleApplication app;

	try {
		app.run();
	} catch(const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
