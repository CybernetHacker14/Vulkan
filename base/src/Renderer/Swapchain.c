#include "Renderer/Swapchain.h"

#include <vulkan/vulkan.h>
#include <stdlib.h>

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

void createSwapchain() {
}
