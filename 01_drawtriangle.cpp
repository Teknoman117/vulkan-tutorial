#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iterator>

#include "vdeleter.hpp"
#include "vkDebugReportCallbackEXT.hpp"

namespace
{
    const char *TITLE  = "01_drawtriangle";
    const int   WIDTH  = 800;
    const int   HEIGHT = 600;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
    };

#ifdef DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
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
        setupDebugCallback();
    }

    void mainLoop()
    {
        while(!glfwWindowShouldClose(window.get())) {
            glfwPollEvents();
        }
    }

    void createInstance()
    {
        // Get the required vulkan extensions
        auto extensions = std::move(getRequiredExtensions());

        // Fetch the validation layers we watch
        if(enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("Requested validation layers unavailable");
        }

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
        createInfo.enabledExtensionCount   = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount       = 0;

        if(enableValidationLayers) {
            createInfo.enabledLayerCount   = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount   = 0;
        }

        // Create the vulkan instance
        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    // Check for the availabilty of some validation layers
    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        // Get the available validation layers
        std::vector<VkLayerProperties>  availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Convert to a set
        std::unordered_set<std::string> availableLayersSet;
        std::for_each(availableLayers.begin(), availableLayers.end(), [&availableLayersSet] (VkLayerProperties& p) {
            availableLayersSet.emplace(p.layerName);
        });

        // Construct other set
        std::unordered_set<std::string> validationLayersSet(validationLayers.begin(), validationLayers.end());

        // Ensure selected validation layers is a subset of available layers
        return std::includes(availableLayersSet.begin(), availableLayersSet.end(), 
                             validationLayersSet.begin(), validationLayersSet.end());
    }

    // Get the required extensions to run 
    std::vector<const char*> getRequiredExtensions() 
    {
        std::vector<const char *> extensions;

        // Get the required vulkan extensions for GLFW to function
        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        extensions.insert(extensions.end(), glfwExtensions, glfwExtensions+glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }

    // Debugging callback
    static VkBool32 debugCallback(VkDebugReportFlagsEXT flags,
                                  VkDebugReportObjectTypeEXT objType,
                                  uint64_t obj,
                                  size_t location,
                                  int32_t code,
                                  const char* layerPrefix,
                                  const char* msg,
                                  void* userData) 
    {
        std::cerr << "validation layer: " << msg << std::endl;
        return VK_FALSE;
    }

    // Setup the debug callback
    void setupDebugCallback()
    {
        if(!enableValidationLayers) {
            return;
        }

        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT) debugCallback;

        if(CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
            throw std::runtime_error("Failed to setup debug callback!");
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
    VDeleter<VkInstance>               instance {vkDestroyInstance};
    VDeleter<VkDebugReportCallbackEXT> callback {instance, DestroyDebugReportCallbackEXT};

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
