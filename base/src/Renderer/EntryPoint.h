#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkInstance_T* VkInstance;

void createInstance();

void destroyInstance();

VkInstance getInstance();

#endif  // !ENTRY_POINT_H
