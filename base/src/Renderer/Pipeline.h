#ifndef PIPELINE_H
#define PIPELINE_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct VkPipeline_T* VkPipeline;

void createPipeline();

void destroyPipeline();

VkPipeline getPipeline();

#endif  // !PIPELINE_H
