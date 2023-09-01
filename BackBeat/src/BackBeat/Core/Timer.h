#pragma once

#include <chrono>

namespace BackBeat {

	class Timer
	{
	public:
		Timer() {};
		~Timer() {};

		void Start() {
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float GetTime() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float GetTimeMs() {
			return GetTime() * 1000.0f;
		}

		void Reset() {
			Start();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}