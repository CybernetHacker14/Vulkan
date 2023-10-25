project "Base"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.c"
    }

    includedirs
    {
        "src",
        "../vendor/glfw/module/include",
        "../vendor/glm/module"
    }

    links
    {
        "glfw"
    }

    filter "system:Windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "VKB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "VKB_RELEASE"
        runtime "Release"
        optimize "on"