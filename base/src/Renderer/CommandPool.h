#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkCommandBuffer_T* VkCommandBuffer;

void recordCommandBuffer(unsigned int imageIndex);

VkCommandBuffer getCommandBuffer();

void createCommandPool();

void createCommandBuffer();

void destroyCommandPool();

#endif  // !COMMANDPOOL_H
