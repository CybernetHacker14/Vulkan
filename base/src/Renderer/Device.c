#include "Renderer/Device.h"
#include "Renderer/EntryPoint.h"
#include "Renderer/DebugUtils.h"

#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdio.h>

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device = VK_NULL_HANDLE;
VkQueue graphicsQueue;

typedef struct QueueFamilyIndices {
    union {
        unsigned int graphicsFamily;
        short nullValue;
    } data;

} QueueFamilyIndices;

int isComplete(QueueFamilyIndices indices) {
    return indices.data.nullValue == -1 ? 0 : 1;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices = {.data.nullValue = -1};

    unsigned int queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies =
        (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));

    if (queueFamilies) {
        for (int i = 0; i < queueFamilyCount; ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.data.graphicsFamily = i;
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

    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = NULL;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = indices.data.graphicsFamily;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {0};

    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
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

    vkGetDeviceQueue(device, indices.data.graphicsFamily, 0, &graphicsQueue);
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
