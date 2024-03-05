workspace "jage"
    startproject "jageeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "jageeditor"
    location "jageeditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    flags
    {
        "FatalWarnings"
    }