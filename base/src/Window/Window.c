#include "Window/Window.h"
#include "Renderer/EntryPoint.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>

unsigned int m_Width = 800;
unsigned int m_Height = 600;

GLFWwindow* window;
VkSurfaceKHR surface;

void initWindow(const unsigned int width, const unsigned int height) {
    m_Width = width;
    m_Height = height;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(m_Width, m_Height, "Hello Vulkan Triangle", NULL, NULL);
}

void createSurface() {
    if (glfwCreateWindowSurface(getInstance(), window, NULL, &surface) != VK_SUCCESS) {
        printf_s("Failed to create window surface!\n");
        return;
    }
}

int isWindowClosed() {
    return glfwWindowShouldClose(window);
}

int isWindowMinimized() {
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED);
}

void onWindowUpdate() {
    glfwPollEvents();
}

void destroySurface() {
    vkDestroySurfaceKHR(getInstance(), surface, NULL);
}

void cleanupWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

VkSurfaceKHR getSurface() {
    return surface;
}

void* getWindow() {
    return window;
}
