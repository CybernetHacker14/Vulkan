project "Base"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.c",
        "shaders/**.vert",
        "shaders/**.frag"
    }

    includedirs
    {
        "src",
        "../vendor/glfw/module/include",
        "../vendor/glm/module",
        "$(VULKAN_SDK)/include"
    }

    links
    {
        "glfw"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS"
        }

        links
        {
            os.getenv("VK_SDK_PATH") .. "/Lib/vulkan-1.lib"
        }

    filter "configurations:Debug"
        defines "VKB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "VKB_RELEASE"
        runtime "Release"
        optimize "on"

    filter "files:**vert"
        flags "ExcludeFromBuild"

    filter "files:**frag"
        flags "ExcludeFromBuild"