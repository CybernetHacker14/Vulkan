include "clean.lua"

workspace "Vulkan"
    architecture "x86_64"
    startproject "Base"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "Base"

    group "Dependencies"
        include "vendor/glfw"
    group ""