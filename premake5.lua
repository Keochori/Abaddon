workspace "Abaddon"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

project "Abaddon"
    location "Abaddon"
    kind "WindowedApp"
    language "C++"

    pchheader "pch.h"
	pchsource "Abaddon/src/pch.cpp"
    
    defines {
        "UNICODE"
    }
    
    links {
        "d3d11.lib",
        "D3DCompiler.lib"
    }
    
    files {
        "%{prj.name}/Resources/*",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hlsl"
    }

    includedirs {
        "%{prj.name}/src"
    }

    targetdir ("bin/%{cfg.buildcfg}")
	objdir ("bin-int/%{cfg.buildcfg}")

    -- Shader options
    shaderobjectfileoutput ("%%(Filename).cso")

    filter("files:**_vs.hlsl")
    shadertype("Vertex")
    
    filter("files:**_ps.hlsl")
    shadertype("Pixel")
    -----------------------

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"