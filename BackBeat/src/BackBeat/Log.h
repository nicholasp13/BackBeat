#pragma once

#include <memory>

#include <stdio.h>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// Log wrapper class for flexibility if needed for changing the logging API

namespace BackBeat {
	
	class BACKBEAT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;


	};
}


// Core Log macros
#define BB_CORE_TRACE(...)   ::BackBeat::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BB_CORE_INFO(...)    ::BackBeat::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BB_CORE_WARN(...)    ::BackBeat::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BB_CORE_ERROR(...)   ::BackBeat::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BB_CORE_FATAL(...)   ::BackBeat::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core Log macros
#define BB_CLIENT_TRACE(...) ::BackBeat::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BB_CLIENT_INFO(...)  ::BackBeat::Log::GetClientLogger()->info(__VA_ARGS__)
#define BB_CLIENT_WARN(...)  ::BackBeat::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BB_CLIENT_ERROR(...) ::BackBeat::Log::GetClientLogger()->error(__VA_ARGS__)
#define BB_CLIENT_FATAL(...) ::BackBeat::Log::GetClientLogger()->fatal(__VA_ARGS__)