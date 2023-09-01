#pragma once


// Makes sure that the platform the application is run is Windows

#ifdef BB_PLATFORM_WINDOWS

	#if BB_DYNAMIC_LINK
		#ifdef BB_BUILD_DLL
			#define BACKBEAT_API _declspec(dllexport)
		#else
			#define BACKBEAT_API _declspec(dllimport)
		#endif
	#else
		#define BACKBEAT_API
	#endif

#else
	#error BackBeat only supports Windows
#endif 

#ifdef BB_DEBUG
	#define BB_ENABLE_ASSERTS
#endif

#ifdef BB_ENABLE_ASSERTS
	#define BB_CLIENT_ASSERTS(x, ...) { if(!(x)) {BB_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BB_CORE_ASSERTS(x, ...) { if(!(x)) { BB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BB_CLIENT_ASSERTS(x, ...)
	#define BB_CORE_ASSERTS(x, ...)
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)

#include "Log.h"