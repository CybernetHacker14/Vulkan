#include "Renderer/Shader.h"
#include "Renderer/Device.h"

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkShaderModule vertShaderModule;
VkShaderModule fragShaderModule;

void readSPRIRVBinary(const char* path, char* buffer, long* length) {
    FILE* file;
    if (fopen_s(&file, path, "rb") != NULL) { printf_s("Failed to open shader file\n"); }

    if (file) {
        fseek(file, 0, SEEK_END);
        *length = ftell(file, 0, SEEK_END);
        fseek(file, 0, SEEK_SET);

        if (buffer != NULL) fread(buffer, sizeof(char), *length, file);

        fclose(file);
    }
}

VkShaderModule createShaderModule(const char* code, long length) {
    VkShaderModuleCreateInfo createInfo;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = length;
    createInfo.pCode = (const unsigned int*)code;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(getLogicalDevice(), &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
        printf_s("Failed to create shader module!\n");
        return NULL;
    }

    return shaderModule;
}

void createShaderModules(const char* vertSPVPath, const char* fragSPVPath) {
    long vertShaderLength = 0;
    long fragShaderLength = 0;
    readSPRIRVBinary(vertSPVPath, NULL, &vertShaderLength);
    readSPRIRVBinary(fragSPVPath, NULL, &fragShaderLength);

    char* vertShaderCode = (char*)malloc(vertShaderLength * sizeof(char));
    char* fragShaderCode = (char*)malloc(fragShaderLength * sizeof(char));
    readSPRIRVBinary(vertSPVPath, vertShaderCode, &vertShaderLength);
    readSPRIRVBinary(fragSPVPath, fragShaderCode, &fragShaderLength);

    vertShaderModule = createShaderModule(vertShaderCode, vertShaderLength);
    fragShaderModule = createShaderModule(fragShaderCode, fragShaderLength);

    free(vertShaderCode);
    free(fragShaderCode);
}

void destroyShaderModules() {
    vkDestroyShaderModule(getLogicalDevice(), fragShaderModule, NULL);
    vkDestroyShaderModule(getLogicalDevice(), vertShaderModule, NULL);
}

VkShaderModule getVertexShaderModule() {
    return vertShaderModule;
}

VkShaderModule getFragmentShaderModule() {
    return fragShaderModule;
}
