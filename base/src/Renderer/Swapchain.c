#include "Renderer/Swapchain.h"
#include "Renderer/Device.h"

#include "Window/Window.h"

#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define max(a, b)        a > b ? a : b
#define min(a, b)        a < b ? a : b
#define clamp(x, lo, hi) min(hi, max(lo, x))

VkSwapchainKHR swapChain;
VkImage* swapChainImages;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details = {0};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    details.formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &(details.formatCount), NULL);

    if (details.formatCount != 0) {
        details.formats = (VkSurfaceFormatKHR*)malloc(details.formatCount * sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &(details.formatCount), details.formats);
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &(details.presentModeCount), NULL);

    if (details.presentModeCount != 0) {
        details.presentModes = (VkPresentModeKHR*)malloc(details.presentModeCount * sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &(details.presentModeCount), details.presentModes);
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(unsigned int formatCount, VkSurfaceFormatKHR* availableFormats) {
    for (int i = 0; i < formatCount; ++i) {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormats[i];
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(unsigned int presentModeCount, VkPresentModeKHR* availablePresentModes) {
    for (int i = 0; i < presentModeCount; ++i) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentModes[i];
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities) {
    if (capabilities.currentExtent.width != UINT_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize((GLFWwindow*)getWindow(), &width, &height);

        VkExtent2D actualExtent = {.width = (unsigned int)width, .height = (unsigned int)height};
        actualExtent.width =
            clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height =
            clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void createSwapchain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(getPhysicalDevice(), getSurface());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formatCount, swapChainSupport.formats);
    VkPresentModeKHR presentMode =
        chooseSwapPresentMode(swapChainSupport.presentModeCount, swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    unsigned int imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = getSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(getPhysicalDevice());
    unsigned int queueFamilyIndices[] = {indices.graphicsFamily.data, indices.presentFamily.data};

    if (indices.graphicsFamily.data != indices.presentFamily.data) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;  // optional
        createInfo.pQueueFamilyIndices = NULL;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(getLogicalDevice(), &createInfo, NULL, &swapChain) != VK_SUCCESS) {
        printf_s("Failed to create swapchain!\n");
    }

    unsigned int swapChainImageCount = 0;
    vkGetSwapchainImagesKHR(getLogicalDevice(), swapChain, &swapChainImageCount, NULL);

    swapChainImages = (VkImage*)malloc(swapChainImageCount * sizeof(VkImage));
    vkGetSwapchainImagesKHR(getLogicalDevice(), swapChain, &swapChainImageCount, swapChainImages);

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void destroySwapchain() {
    free(swapChainImages);

    vkDestroySwapchainKHR(getLogicalDevice(), swapChain, NULL);
}
