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

include "BackBeat/vendor/GLFW"
include "BackBeat/vendor/Glad"

project "BackBeat"
	location "BackBeat"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bbpch.h"
	pchsource "BackBeat/src/bbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		-- CURRENT SOL
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include"
		--TO FIX --
		-- "%IncludeDir.GLFW",
		-- "%IncludeDir.Glad"
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		-- Might need to change C++ version --
		cppdialect "C++20"
		staticruntime "Off"
		runtime "Release"
		systemversion "latest"

		defines
		{
			"BB_PLATFORM_WINDOWS",
			"BB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sampler")
		}

	filter "configurations:Debug"
		defines "BB_DEBUG"
		symbols "On"

	filter "configurations:RELEASE"
		defines "BB_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "BB_DIST"
		optimize "On"


project "Sampler"
	location "Sampler"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BackBeat/vendor/spdlog/include",
		"BackBeat/src"
	}
	
	links
	{
		"BackBeat"
	}

	filter "system:windows"
		-- Might need to change C++ version --
		cppdialect "C++20"
		staticruntime "Off"
		runtime "Release"
		systemversion "latest"

		defines
		{
			"BB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BB_DEBUG"
		symbols "On"

	filter "configurations:RELEASE"
		defines "BB_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "BB_DIST"
		optimize "On"