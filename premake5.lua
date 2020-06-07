workspace "BlackBirdBox"
    architecture "x64"
    startproject "ClothSimulation"

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
IncludeDir["rapidjson"] = "external/rapidjson/include/rapidjson"

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
		"%{IncludeDir.tinyobjloader}/tiny_obj_loader.h",
		"%{IncludeDir.rapidjson}/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC"
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
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.rapidjson}"
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

project "ClothSimulation"
	location "ClothSimulation"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	--libdirs { "external/BlackBirdBox/external/glew/lib/Release/x64" }

	includedirs
	{
		"src",
		"external",
		"external/glm",
		"external/spdlog/include",
		"external/imgui",
		"%{IncludeDir.rapidjson}"
	}

	links 
	{ 
		"BlackBirdBox",
    }
    
    filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"