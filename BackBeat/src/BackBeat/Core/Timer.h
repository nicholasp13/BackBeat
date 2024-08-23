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

		float GetTime() 
		{
			return GetTimeNano() * 0.001f * 0.001f * 0.001f;
		}

		float GetTimeMs()
		{
			return GetTime() * 0.001f * 0.001f;
		}

		float GetTimeMicro() 
		{
			return GetTimeNano() * 0.001f;
		}

		float GetTimeNano() 
		{
			return (float)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count();
		}

		void Reset() 
		{
			Start();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}