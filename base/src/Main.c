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
#endif // VKB_DEBUG

const char* validationLayers[1] = {
	"VK_LAYER_KHRONOS_validation"
};

GLFWwindow* window = NULL;
VkInstance instance;

int validateglfwExtensions(unsigned int glfwExtensionCount, const char** glfwExtensions) {

	unsigned int vkExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionCount, NULL);
	VkExtensionProperties* vkExtensions = (VkExtensionProperties*)malloc(vkExtensionCount * sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionCount, vkExtensions);

	if (!vkExtensions) {
		return 0;
	}

	int extensionsFound = 0;
	for (int i = 0; i < vkExtensionCount; ++i) {

		for (int j = 0; j < glfwExtensionCount; ++j) {
			if (strcmp(vkExtensions[i].extensionName, glfwExtensions[j]) == 0) {
				extensionsFound++;
			}
		}
	}

	free(vkExtensions);

	if (extensionsFound < glfwExtensionCount) {
		return 0;
	}

	return 1;
}

int checkValidationLayerSupport() {
	unsigned int layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);

	VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

	if (!availableLayers) {
		free(availableLayers);
		return 0;
	}

	for (int i = 0; i < 1; ++i) {
		short layerFound = 0;

		for (int j = 0; j < layerCount; ++j) {
			if (strcmp(availableLayers[j].layerName, validationLayers[i]) == 0) {
				layerFound = 1;
				break;
			}
		}

		if (!layerFound) {
			free(availableLayers);
			return 0;
		}
	}

	free(availableLayers);
	return 1;
}

void createInstance() {
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	if (!validateglfwExtensions(glfwExtensionCount, glfwExtensions)) {
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
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = enableValidationLayers ? (unsigned int)1 : 0; // Not a bool, its a count. We have manually specified it as 1
	createInfo.ppEnabledLayerNames = enableValidationLayers ? &validationLayers : NULL;

	if (vkCreateInstance(&createInfo, NULL, &instance)) {
		printf("Failed to create instance");
		return;
	}
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
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
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