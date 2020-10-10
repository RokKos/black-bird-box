project "ClothSimulation"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	--libdirs { "external/BlackBirdBox/external/glew/lib/Release/x64" }

	includedirs
	{
		"../BlackBirdBox/src",
		"../%{IncludeDir.glm}",
		"../external/spdlog/include",
        "../%{IncludeDir.ImGui}",
		"../%{IncludeDir.rapidjson}",
		"../%{IncludeDir.soloud}"
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