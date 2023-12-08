#ifndef WINDOW_H
#define WINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkSurfaceKHR_T* VkSurfaceKHR;

void initWindow(const unsigned int width, const unsigned int height);

void createSurface();

int isWindowClosed();

int isWindowMinimized();

void onWindowUpdate();

void destroySurface();

void cleanupWindow();

VkSurfaceKHR getSurface();

void* getWindow();

#endif  // !WINDOW_H
