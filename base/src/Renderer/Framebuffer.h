#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkFramebuffer_T* VkFramebuffer;

void createFramebuffers();

void destroyFramebuffers();

VkFramebuffer* getSwapChainFramebuffers();

#endif  // !FRAMEBUFFER_H
