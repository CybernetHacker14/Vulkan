#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

#ifdef VKB_DEBUG
const short enableValidationLayers = 1;
#else
const short enableValidationLayers = 0;
#endif  // VKB_DEBUG

#define LAYERCOUNT 1

const char* validationLayers[LAYERCOUNT] = {"VK_LAYER_KHRONOS_validation"};

GLFWwindow* window = NULL;
VkInstance instance;

void getRequiredExtensions(char** extensions, unsigned int count) {
    unsigned int temp = 0;

    for (int i = 0; i < (enableValidationLayers ? count - LAYERCOUNT : count); ++i)
        extensions[i] = glfwGetRequiredInstanceExtensions(&temp)[i];

    if (enableValidationLayers) extensions[count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
}

int validateglfwExtensions(unsigned int glfwExtensionCount, const char** glfwExtensions) {
    unsigned int vkExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionCount, NULL);
    VkExtensionProperties* vkExtensions =
        (VkExtensionProperties*)malloc(vkExtensionCount * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionCount, vkExtensions);

    int extensionsFound = 0;

    if (vkExtensions) {
        for (int i = 0; i < vkExtensionCount; ++i) {
            for (int j = 0; j < glfwExtensionCount; ++j)
                if (strcmp(vkExtensions[i].extensionName, glfwExtensions[j]) == 0) extensionsFound++;
        }
    }

    free(vkExtensions);

    return !(extensionsFound < glfwExtensionCount);
}

int checkValidationLayerSupport() {
    unsigned int layerCount;
    int success = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    if (availableLayers) {
        for (int i = 0; i < LAYERCOUNT; ++i) {
            short layerFound = 0;

            for (int j = 0; j < layerCount; ++j) {
                if (strcmp(availableLayers[j].layerName, validationLayers[i]) == 0) {
                    success = layerFound = 1;
                    break;
                }
            }

            if (!layerFound) {
                success = 0;
                break;
            }
        }
    }

    free(availableLayers);

    return success;
}

void createInstance() {
    unsigned int extensionCount = 0;
    char** extensions;

    glfwGetRequiredInstanceExtensions(&extensionCount);
    if (enableValidationLayers) ++extensionCount;

    extensions = (char**)malloc(sizeof(char*) * extensionCount);

    getRequiredExtensions(extensions, extensionCount);

    if (!validateglfwExtensions(extensionCount, extensions)) {
        printf("Extension validation error\n");
        return;
    }

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        printf("Validation layers requested, but not available\n");
        return;
    }

    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;
    createInfo.enabledLayerCount = enableValidationLayers
                                       ? (unsigned int)LAYERCOUNT
                                       : 0;  // Not a bool, its a count. We have manually specified it as 1
    createInfo.ppEnabledLayerNames = enableValidationLayers ? &validationLayers : NULL;

    if (vkCreateInstance(&createInfo, NULL, &instance)) {
        printf("Failed to create instance");
        return;
    }

    free(extensions);
}

void initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Vulkan Triangle", NULL, NULL);
}

void initVulkan() {
    createInstance();
}

void mainLoop() {
    while (!glfwWindowShouldClose(window)) { glfwPollEvents(); }
}

void cleanup() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

int main() {
    run();

    return 0;
}