#include "Window/Window.h"

#include "Renderer/EntryPoint.h"
#include "Renderer/DebugMessenger.h"
#include "Renderer/Device.h"

#include <stdio.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void initVulkan() {
    createInstance();
    createDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}

void mainLoop() {
    while (!isWindowClosed()) { onWindowUpdate(); }
}

void cleanup() {
    destroyLogicalDevice();
    destroyDebugMessenger();
    destroySurface();
    destroyInstance();

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