#include "Window/Window.h"

#include "Renderer/EntryPoint.h"
#include "Renderer/DebugMessenger.h"
#include "Renderer/Device.h"
#include "Renderer/Swapchain.h"

#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderPass.h"

#include "Renderer/Framebuffer.h"
#include "Renderer/CommandPool.h"

#include <stdio.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void createDeviceSetup() {
    createInstance();
    createDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
}

void destroyDeviceSetup() {
    destroyImageViews();
    destroySwapchain();
    destroyLogicalDevice();
    destroyDebugMessenger();
    destroySurface();
    destroyInstance();
}

void createGraphicsPipeline() {
    createShaderModules("shaders/shader_base/shader_base_vert.spv", "shaders/shader_base/shader_base_frag.spv");
    createRenderPass();
    createPipeline();
}

void destroyGraphicsPipeline() {
    destroyPipeline();
    destroyRenderPass();
    destroyShaderModules();
}

void initVulkan() {
    createDeviceSetup();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
}

void mainLoop() {
    while (!isWindowClosed()) { onWindowUpdate(); }
}

void cleanup() {
    destroyCommandPool();
    destroyFramebuffers();
    destroyGraphicsPipeline();
    destroyDeviceSetup();
    cleanupWindow();
}

void run() {
    initWindow(WIDTH, HEIGHT);
    initVulkan();
    mainLoop();
    cleanup();
}

int main() {
    run();

    return 0;
}