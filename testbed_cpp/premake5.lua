project "Testbed_cpp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
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