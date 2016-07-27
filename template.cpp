#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

class HelloTriangleApplication 
{
	void initVulkan()
	{

	}

	void mainLoop()
	{

	}

public:
	void run()
	{
		initVulkan();
		mainLoop();
	}
}

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
