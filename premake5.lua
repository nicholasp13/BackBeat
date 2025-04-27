workspace "BackBeat"
	architecture "x64"
	startproject "Exampler"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "BackBeat/vendor/glfw"
include "BackBeat/vendor/Glad"
include "BackBeat/vendor/imgui"
include "BackBeat/vendor/lodepng"
include "BackBeat/vendor/pugixml"
include "BackBeat/vendor/imgui-knobs"

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
		"%{prj.name}/assets/**.png",
		"%{prj.name}/assets/**.wav"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/lodepng",
		"%{prj.name}/vendor/pugixml",
		"%{prj.name}/vendor/imgui-knobs",
		"%{prj.name}/vendor/fftw3/include"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"lodepng",
		"pugixml",
		"imgui-knobs",
		"opengl32.lib",
		"fftw3f"
	}

	libdirs 
	{ 
		"%{prj.name}/vendor/fftw3/bin"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/utf-8"

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


project "Exampler"
	location "Exampler"
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
		"BackBeat/vendor/pugixml",
		"BackBeat/vendor/imgui-knobs",
		"BackBeat/vendor/fftw3/include",
		"BackBeat/src"
	}
	
	links
	{
		"BackBeat"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/utf-8"

		defines
		{
			"BB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BB_DEBUG"
		symbols "on"
		ignoredefaultlibraries  { "LIBCMT" }

	filter "configurations:RELEASE"
		defines "BB_RELEASE"
		optimize "on"
	
	filter "configurations:Dist"
		defines "BB_DIST"
		optimize "on"

project "UnitTests"
	location "UnitTests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"BackBeat/vendor/spdlog/include",
		"BackBeat/vendor/imgui",
		"BackBeat/vendor/glfw/include",
		"BackBeat/vendor/lodepng",
		"BackBeat/vendor/pugixml",
		"BackBeat/vendor/imgui-knobs",
		"BackBeat/vendor/fftw3/include",
		"BackBeat/src"
	}
	
	links
	{
		"BackBeat"
	}

	nuget
	{
		"Microsoft.googletest.v140.windesktop.msvcstl.static.rt-static:1.8.1.7"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/utf-8"

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