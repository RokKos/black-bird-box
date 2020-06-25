project "BlackBirdBox"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bbbpch.h"
	pchsource "src/bbbpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"../external/stb/stb_image.h",
		"../external/stb/stb_image.cpp",
		"../%{IncludeDir.glm}/glm/**.hpp",
        "../%{IncludeDir.glm}/glm/**.inl",
		"../%{IncludeDir.glew}/**.h",
		"../%{IncludeDir.tinyobjloader}/tiny_obj_loader.h",
		"../%{IncludeDir.rapidjson}/**.h"
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
		"../external/spdlog/include",
		"../%{IncludeDir.GLFW}",
		"../%{IncludeDir.glew}",
		"../%{IncludeDir.ImGui}",
		"../%{IncludeDir.glm}",
        "../%{IncludeDir.stb}", 
		"../%{IncludeDir.tinyobjloader}",
		"../%{IncludeDir.rapidjson}"
	}

    libdirs { "../external/glew/lib/Release/x64" }

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