project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
	targetdir ("bin/" .. TargetDir .. "/%{prj.name}")
    objdir ("bin-int/" .. ObjectDir .. "/%{prj.name}")

	files {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",
        "imgui_tables.cpp"
    }
    
	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
