#include "Renderer/DebugMessenger.h"
#include "Renderer/DebugUtils.h"

#include "Renderer/EntryPoint.h"

#include <vulkan/vulkan.h>

#include <stdio.h>

VkDebugUtilsMessengerEXT debugMessenger;

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

void createDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    if (CreateDebugUtilsMessengerEXT(getInstance(), &createInfo, NULL, &debugMessenger) != VK_SUCCESS) {
        printf_s("Failed to setup debug messenger!\n");
        return;
    }
}

void destroyDebugMessenger() {
    if (!enableValidationLayers) return;

    DestroyDebugUtilsMessengerEXT(getInstance(), debugMessenger, NULL);
}
