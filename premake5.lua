workspace "Abaddon"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "Abaddon"
    location "Abaddon"
    kind "WindowedApp"
    language "C++"

    defines {
        "UNICODE"
    }

    files {
        "%{prj.name}/Resources/*",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs {
        "%{prj.name}/src"
    }

    targetdir ("bin/%{cfg.buildcfg}")
	objdir ("bin-int/%{cfg.buildcfg}")