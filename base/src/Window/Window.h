#ifndef WINDOW_H
#define WINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

void initWindow(const unsigned int width, const unsigned int height);

int isWindowClosed();

void onWindowUpdate();

void cleanupWindow();

#endif  // !WINDOW_H
