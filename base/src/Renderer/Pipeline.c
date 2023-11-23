#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"

#include "vulkan/vulkan.h"

void createPipelineShaderStages() {
    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.pNext = NULL;
    vertShaderStageInfo.flags = 0;
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = getVertexShaderModule();
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.pSpecializationInfo = NULL;

    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.pNext = NULL;
    fragShaderStageInfo.flags = 0;
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = getFragmentShaderModule();
    fragShaderStageInfo.pName = "main";
    fragShaderStageInfo.pSpecializationInfo = NULL;

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
}

void createPipeline() {
    createPipelineShaderStages();
}

void destroyPipeline() {
}
