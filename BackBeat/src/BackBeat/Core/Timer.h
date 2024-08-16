#pragma once

#include <chrono>

namespace BackBeat {

	class Timer
	{
	public:
		Timer() {};
		~Timer() {};

		void Start() 
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		// Seconds
		float GetTime() 
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		// Microseconds
		float GetTimeMicro() {
			return GetTime() * 1000.0f;
		}

		// NanoSeconds
		float GetTimeNano() {
			return (float)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count();
		}

		void Reset() {
			Start();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}