workspace "jage"
    startproject "jageeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- External dependencies
externals = {}
externals["sdl2"] = "external/sdl2"
externals["spdlog"] = "external/spdlog"
externals["glad"] = "external/glad"

-- Process Glad before anything else
include "external/glad"

project "jage"
    location "jage"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "%{prj.name}/include",
        "%{externals.sdl2}/include",
        "%{externals.spdlog}/include",
        "%{externals.glad}/include"
    }

    flags
    {
        -- TODO: Findout why format.h is causing warnings
        -- "FatalWarnings"
    }

    defines
    {
        "GLFW_INCLUDE_NONE" -- Ensures glad doesn't include GLFW
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "JAGE_PLATFORM_WINDOWS"
        }
    
    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "JAGE_PLATFORM_MACOS"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "JAGE_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines
        {
            "JAGE_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines
        {
            "JAGE_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

project "jageeditor"
    location "jageeditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "jage"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "jage/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "JAGE_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2",
            "SDL2main",
            "glad"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "JAGE_PLATFORM_MACOS"
        }

        links
        {
            "SDL2.framework",
            "glad"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "JAGE_PLATFORM_LINUX"
        }

        links 
        {
            "SDL2",
            "SDL2main",
            "glad",
            "dl"
        }

    filter "configurations:Debug"
        defines
        {
            "JAGE_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines
        {
            "JAGE_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"