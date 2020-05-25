workspace "BlackBirdBox"
    architecture "x64"
    startproject "BlackBirdBox"

    configurations
    {
		"Debug",
		"Release",
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "external/GLFW/include"
IncludeDir["glew"] = "external/glew/include"
IncludeDir["ImGui"] = "external/imgui"
IncludeDir["glm"] = "external/glm"
IncludeDir["tinyobjloader"] = "external/tinyobjloader"
IncludeDir["stb"] = "external/stb"

group "Dependencies"
	include "external/GLFW"
	include "external/imgui"

group ""

project "BlackBirdBox"
	location "BlackBirdBox"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bbbpch.h"
	pchsource "src/bbbpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"external/stb/stb_image.h",
		"external/stb/stb_image.cpp",
		"external/glm/glm/**.hpp",
        "external/glm/glm/**.inl",
        "%{IncludeDir.glew}/**.h",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"external/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.stb}", 
        "%{IncludeDir.tinyobjloader}"
	}

    libdirs { "external/glew/lib/Release/x64" }

	links 
	{ 
		"GLFW",
		"glew32s.lib",
		"ImGui",
		"opengl32.lib"
    }
    
    filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "BBB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BBB_RELEASE"
		runtime "Release"
		optimize "on"