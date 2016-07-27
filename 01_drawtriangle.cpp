#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>

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
	}

	void mainLoop()
	{
		while(!glfwWindowShouldClose(window.get())) {
			glfwPollEvents();
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
