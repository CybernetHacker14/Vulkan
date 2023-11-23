#ifndef SHADER_H
#define SHADER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkShaderModule_T* VkShaderModule;

void createShaderModules(const char* vertSPVPath, const char* fragSPVPath);

void destroyShaderModules();

VkShaderModule getVertexShaderModule();

VkShaderModule getFragmentShaderModule();

#endif  // !SHADER_H
