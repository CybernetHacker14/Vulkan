#include "Renderer/CommandPool.h"
#include "Renderer/Device.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Swapchain.h"
#include "Renderer/Pipeline.h"

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

VkCommandPool commandPool;
VkCommandBuffer commandBuffer;

void recordCommandBuffer(unsigned int imageIndex) {
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.pNext = NULL;
    beginInfo.flags = 0;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pInheritanceInfo = NULL;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        printf_s("Failed to begin recording command buffer!\n");
        return;
    }

    VkRenderPassBeginInfo renderPassInfo;
    renderPassInfo.pNext = NULL;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = getRenderPass();
    renderPassInfo.framebuffer = getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = getSwapchainExtent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipeline());
        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)(getSwapchainExtent().width);
        viewport.height = (float)(getSwapchainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent = getSwapchainExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        printf_s("Failed to record command buffer!\n");
        return;
    }
}

VkCommandBuffer getCommandBuffer() {
    return commandBuffer;
}

void createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo;
    poolInfo.pNext = NULL;
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.data;

    if (vkCreateCommandPool(getLogicalDevice(), &poolInfo, NULL, &commandPool) != VK_SUCCESS) {
        printf_s("Failed to create command pool!\n");
        return;
    }
}

void createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.pNext = NULL;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(getLogicalDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
        printf_s("Failed to create command buffers!\n");
        return;
    }
}

void destroyCommandPool() {
    vkDestroyCommandPool(getLogicalDevice(), commandPool, NULL);
}
