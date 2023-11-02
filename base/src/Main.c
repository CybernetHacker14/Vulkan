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
VkDebugUtilsMessengerEXT debugMessenger;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    fprintf_s(stderr, "Validation layer: %s \n", pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    return func != NULL ? func(instance, pCreateInfo, pAllocator, pDebugMessenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) func(instance, debugMessenger, pAllocator);
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo) {
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->pNext = NULL;
    createInfo->flags = 0;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
    createInfo->pUserData = NULL;
}

void setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, NULL, &debugMessenger) != VK_SUCCESS) {
        printf("Failed to setup debug messenger!\n");
        return;
    }
}

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

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = enableValidationLayers
                                           ? (unsigned int)LAYERCOUNT
                                           : 0;  // Not a bool, its a count. We have manually specified it as 1
        createInfo.ppEnabledLayerNames = enableValidationLayers ? &validationLayers : NULL;

        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = NULL;
    }

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
    setupDebugMessenger();
}

void mainLoop() {
    while (!glfwWindowShouldClose(window)) { glfwPollEvents(); }
}

void cleanup() {
    if (enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);

    vkDestroyInstance(instance, NULL);

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