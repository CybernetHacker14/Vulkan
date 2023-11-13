#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include <vulkan/vulkan_core.h>

#ifdef VKB_DEBUG
static const short enableValidationLayers = 1;
#else
static const short enableValidationLayers = 0;
#endif  // VKB_DEBUG

// Since this is getting used in both general purpose DebugMessenger class and EntryPoint class, had to make this
// separate Need to find a better way of how things are done in C

// Right now, this should be used only internally

#define DEBUG_LAYERCOUNT 1
static const char* validationLayers[DEBUG_LAYERCOUNT] = {"VK_LAYER_KHRONOS_validation"};

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);

#endif  // !DEBUG_UTILS_H
