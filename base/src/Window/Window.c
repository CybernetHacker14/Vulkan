#include "Window/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

unsigned int m_Width = 800;
unsigned int m_Height = 600;

GLFWwindow* window;

void initWindow(const unsigned int width, const unsigned int height) {
    m_Width = width;
    m_Height = height;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(m_Width, m_Height, "Hello Vulkan Triangle", NULL, NULL);
}

int isWindowClosed() {
    return glfwWindowShouldClose(window);
}

void onWindowUpdate() {
    glfwPollEvents();
}

void cleanupWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
