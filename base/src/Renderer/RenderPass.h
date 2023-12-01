#ifndef RENDERPASS_H
#define RENDERPASS_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkRenderPass_T* VkRenderPass;

void createRenderPass();

void destroyRenderPass();

VkRenderPass getRenderPass();

#endif  // !RENDERPASS_H
