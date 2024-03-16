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
    cppdialect "C++20"

    pchheader "pch.h"
	pchsource "Abaddon/src/pch.cpp"
    
    TargetDir = "bin/%{cfg.buildcfg}"
    ObjectDir = "bin-int/%{cfg.buildcfg}"
    
    targetdir (TargetDir)
    objdir (ObjectDir)

    defines {
        "UNICODE"
    }
    
    links {
        "d3d11.lib",
        "D3DCompiler.lib",
        "lib/DirectXTex.lib"
    }
    
    files {
        "%{prj.name}/Resources/*",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.hlsl",

        "%{prj.name}/vendor/**.cpp",
        "%{prj.name}/vendor/**.h",
        "%{prj.name}/vendor/**.hpp",
        "%{prj.name}/vendor/**.inl"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor"
    }

    -- Shader options
    shaderobjectfileoutput ("../Assets/Shaders/%%(Filename).cso")

    filter("files:**_vs.hlsl")
    shadertype("Vertex")
    shadermodel "5.0"
    
    filter("files:**_ps.hlsl")
    shadertype("Pixel")
    shadermodel "5.0"
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