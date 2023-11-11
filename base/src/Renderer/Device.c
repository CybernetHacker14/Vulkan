#include "Renderer/Device.h"
#include "Renderer/EntryPoint.h"
#include "Renderer/DebugUtils.h"

#include "Window/Window.h"

#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdio.h>

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device = VK_NULL_HANDLE;
VkQueue graphicsQueue;
VkQueue presentQueue;

typedef struct optional_uint {
    unsigned short flag;
    unsigned int data;
} optional;

typedef struct QueueFamilyIndices {
    optional graphicsFamily;
    optional presentFamily;

} QueueFamilyIndices;

int isComplete(QueueFamilyIndices indices) {
    return indices.graphicsFamily.flag == 1 && indices.presentFamily.flag == 1;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices = {
        .graphicsFamily.data = 0, .graphicsFamily.flag = 0, .presentFamily.data = 0, .presentFamily.flag = 0};

    unsigned int queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies =
        (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    if (queueFamilies) {
        for (int i = 0; i < queueFamilyCount; ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !indices.graphicsFamily.flag) {
                indices.graphicsFamily.data = i;
                indices.graphicsFamily.flag = 1;
                continue;
            }

            VkBool32 presentSupport = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, getSurface(), &presentSupport);

            if (presentSupport) {
                indices.presentFamily.data = i;
                indices.presentFamily.flag = 1;
            }

            if (isComplete(indices)) break;
        }
    }

    free(queueFamilies);

    return indices;
}

int isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);
    return isComplete(indices);
}

void pickPhysicalDevice() {
    unsigned int deviceCount = 0;
    vkEnumeratePhysicalDevices(getInstance(), &deviceCount, NULL);

    if (deviceCount == 0) {
        printf_s("Failed to find GPUs with Vulkan support!\n");
        return;
    }

    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));

    vkEnumeratePhysicalDevices(getInstance(), &deviceCount, devices);

    if (devices) {
        for (int i = 0; i < deviceCount; ++i) {
            if (isDeviceSuitable(devices[i])) {
                physicalDevice = devices[i];
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            printf_s("Failed to find a suitable GPU!\n");
            return;
        }
    }

    free(devices);
}

void createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfos[2];
    unsigned int uniqueQueueFamilies[2] = {indices.graphicsFamily.data, indices.presentFamily.data};

    float queuePriority = 1.0f;

    for (int i = 0; i < 2; ++i) {
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].pNext = NULL;
        queueCreateInfos[i].flags = 0;
        queueCreateInfos[i].queueFamilyIndex = uniqueQueueFamilies[i];
        queueCreateInfos[i].queueCount = 1;
        queueCreateInfos[i].pQueuePriorities = &queuePriority;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {0};

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = 2;
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = LAYERCOUNT;
        createInfo.ppEnabledLayerNames = validationLayers;

    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDeviceCreateInfo* createInfoPtr = &createInfo;

    if (vkCreateDevice(physicalDevice, &createInfo, NULL, &device) != VK_SUCCESS) {
        printf_s("Failed to create logical device!\n");
        return;
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.data, 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.data, 0, &presentQueue);
}

void destroyLogicalDevice() {
    vkDestroyDevice(device, NULL);
}

VkPhysicalDevice getPhysicalDevice() {
    return physicalDevice;
}

VkDevice getLogicalDevice() {
    return device;
}

VkQueue getGraphicsQueue() {
    return graphicsQueue;
}
