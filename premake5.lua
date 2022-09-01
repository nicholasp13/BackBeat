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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		-- Might need to change C++ version --
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BB_PLATFORM_WINDOWS",
			"BB_BUILD_DLL"
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
		staticruntime "On"
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