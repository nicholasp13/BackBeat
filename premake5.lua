workspace "BackBeat"
	architecture "x64"
	startproject "Sampler"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- TO FIX --
-- IncludeDir = {}
-- IncludeDir["GLFW"] = "BackBeat/vendor/GLFW/include"
-- IncludeDir["Glad"] = "BackBeat/vendor/Glad/include"

include "BackBeat/vendor/glfw"
include "BackBeat/vendor/Glad"
include "BackBeat/vendor/imgui"
include "BackBeat/vendor/lodepng"

project "BackBeat"
	location "BackBeat"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bbpch.h"
	pchsource "BackBeat/src/bbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**.png"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		-- CURRENT SOL
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/lodepng"
		--TO FIX --
		-- "%IncludeDir.GLFW",
		-- "%IncludeDir.Glad"
	}

	links{
		"GLFW",
		"Glad",
		"imgui",
		"lodepng",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BB_PLATFORM_WINDOWS",
			"BB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "BB_DEBUG"
		symbols "on"

	filter "configurations:RELEASE"
		defines "BB_RELEASE"
		optimize "on"
	
	filter "configurations:Dist"
		defines "BB_DIST"
		optimize "on"


project "Sampler"
	location "Sampler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**.mp3",
		"%{prj.name}/assets/**.wav"
	}

	includedirs
	{
		"BackBeat/vendor/spdlog/include",
		"BackBeat/vendor/imgui",
		"BackBeat/vendor/glfw/include",
		"BackBeat/vendor/lodepng",
		"BackBeat/src"
	}
	
	links
	{
		"BackBeat"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BB_DEBUG"
		symbols "on"

	filter "configurations:RELEASE"
		defines "BB_RELEASE"
		optimize "on"
	
	filter "configurations:Dist"
		defines "BB_DIST"
		optimize "on"