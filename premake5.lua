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
IncludeDir["rapidjson"] = "external/rapidjson/include"

group "Dependencies"
	include "external/GLFW"
	include "external/imgui"

group ""

include "BlackBirdBox"
include "ClothSimulation"