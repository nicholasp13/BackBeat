#pragma once

#ifdef BB_PLATFORM_WINDOWS
	#ifdef BB_BUILD_DLL
		#define BACKBEAT_API _declspec(dllexport)
	#else
		#define BACKBEAT_API _declspec(dllimport)
	#endif
#else
	#error BackBeat only supports Windows
#endif 