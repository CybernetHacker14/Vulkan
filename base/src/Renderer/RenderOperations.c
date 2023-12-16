#include "Renderer/RenderOperations.h"
#include "Renderer/Device.h"
#include "Renderer/Swapchain.h"
#include "Renderer/CommandPool.h"

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkSemaphore imageAvailableSemaphore;
VkSemaphore renderFinishedSemaphore;
VkFence inFlightFence;

void createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext = NULL;
    semaphoreInfo.flags = 0;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (vkCreateSemaphore(getLogicalDevice(), &semaphoreInfo, NULL, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(getLogicalDevice(), &semaphoreInfo, NULL, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(getLogicalDevice(), &fenceInfo, NULL, &inFlightFence) != VK_SUCCESS) {
        printf_s("Failed to create synchronization primitives");
        return;
    }
}

void destroySyncObjects() {
    vkDestroySemaphore(getLogicalDevice(), imageAvailableSemaphore, NULL);
    vkDestroySemaphore(getLogicalDevice(), renderFinishedSemaphore, NULL);
    vkDestroyFence(getLogicalDevice(), inFlightFence, NULL);
}

void drawFrame() {
    vkWaitForFences(getLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(getLogicalDevice(), 1, &inFlightFence);

    unsigned int imageIndex;
    vkAcquireNextImageKHR(
        getLogicalDevice(), getSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkCommandBuffer buffer = getCommandBuffer();
    vkResetCommandBuffer(buffer, 0);
    recordCommandBuffer(imageIndex);

    VkSubmitInfo submitInfo = {0};
    submitInfo.pNext = NULL;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        printf_s("Failed to submit draw command buffer!\n");
        return;
    }

    VkPresentInfoKHR presentInfo;
    presentInfo.pNext = NULL;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {getSwapchain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = NULL;

    vkQueuePresentKHR(getPresentQueue(), &presentInfo);
}

void waitBeforeExiting() {
    vkDeviceWaitIdle(getLogicalDevice());
}
