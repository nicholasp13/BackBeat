#pragma once

#include "BackBeat/Core/Timer.h"

// Uses both spinlock and sleep to min max sleeping while making sure the thread stays awake
// or is awake at critical times with major accuracy
namespace BackBeat {

	typedef long long nanoseconds;

	class SpinSleeper
	{
	public:
		SpinSleeper();
		~SpinSleeper();

		void Spin(nanoseconds time);
		void Sleep(nanoseconds time);
		void SpinSleep(nanoseconds totalTime, nanoseconds sleepTime);
		
		void Stop() { m_Running = false; }
		inline bool IsRunning() { return m_Running; }

	private:
		bool m_Running;
		Timer m_Timer;

	};

}