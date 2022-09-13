#pragma once

// Makes sure that the platform the application is run is Windows

#ifdef BB_PLATFORM_WINDOWS
	#ifdef BB_BUILD_DLL
		#define BACKBEAT_API _declspec(dllexport)
	#else
		#define BACKBEAT_API _declspec(dllimport)
	#endif
#else
	#error BackBeat only supports Windows
#endif 

#ifdef BB_ENABLE_ASSERTS
	#define BB_CLIENT_ASSERTS(x, ...) { if(!(x)) {BB_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BB_CORE_ASSERTS(x, ...) { if(!(x)) { BB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BB_CLIENT_ASSERTS(x, ...)
	#define BB_CORE_ASSERTS(x, ...)
#endif

#define BIT(x) (1 << x)

#include "Log.h"