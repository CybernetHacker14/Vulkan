#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include <vulkan/vulkan_core.h>

typedef struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    unsigned int formatCount;
    VkSurfaceFormatKHR* formats;
    unsigned int presentModeCount;
    VkPresentModeKHR* presentModes;
} SwapChainSupportDetails;

// Remember to free SwapChainSupportDetails.formats and SwapChainSupportDetails.presentModes,
// because we are allocating it on heap using malloc
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

void createSwapchain();

void createImageViews();

void destroyImageViews();

void destroySwapchain();

#endif  // !SWAPCHAIN_H
