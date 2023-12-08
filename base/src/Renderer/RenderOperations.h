#ifndef RENDEROPERATIONS_H
#define RENDEROPERATIONS_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

void createSyncObjects();

void destroySyncObjects();

void drawFrame();

void waitBeforeExiting();

#endif  // !RENDEROPERATIONS_H
