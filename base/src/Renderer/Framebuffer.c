#include "Renderer/Framebuffer.h"
#include "Renderer/Device.h"
#include "Renderer/Swapchain.h"
#include "Renderer/RenderPass.h"

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int swapChainFramebufferCount;
VkFramebuffer* swapChainFramebuffers;

void createFramebuffers() {
    swapChainFramebufferCount = getSwapchainImageCount();

    swapChainFramebuffers = (VkFramebuffer*)malloc(swapChainFramebufferCount * sizeof(VkFramebuffer));

    if (swapChainFramebuffers) {
        for (unsigned int i = 0; i < swapChainFramebufferCount; ++i) {
            VkImageView attachments[] = {getSwapchainImageViews()[i]};

            VkFramebufferCreateInfo framebufferInfo;
            framebufferInfo.pNext = NULL;
            framebufferInfo.flags = 0;
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = getRenderPass();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = getSwapchainExtent().width;
            framebufferInfo.height = getSwapchainExtent().height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(getLogicalDevice(), &framebufferInfo, NULL, &swapChainFramebuffers[i]) !=
                VK_SUCCESS) {
                printf_s("failed to create framebuffer!\n");
                return;
            }
        }
    }
}

void destroyFramebuffers() {
    for (unsigned int i = 0; i < swapChainFramebufferCount; ++i) {
        vkDestroyFramebuffer(getLogicalDevice(), swapChainFramebuffers[i], NULL);
    }

    free(swapChainFramebuffers);
}
