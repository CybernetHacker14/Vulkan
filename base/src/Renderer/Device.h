#ifndef DEVICE_H
#define DEVICE_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkQueue_T* VkQueue;

void pickPhysicalDevice();

void createLogicalDevice();

void destroyLogicalDevice();

VkPhysicalDevice getPhysicalDevice();

VkDevice getLogicalDevice();

VkQueue getGraphicsQueue();

#endif  // !DEVICE_H
