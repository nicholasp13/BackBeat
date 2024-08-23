#include "bbpch.h"

#include "BackBeat/Core/Log.h"
#include "SpinSleeper.h"
namespace BackBeat {

	SpinSleeper::SpinSleeper()
		: m_Running(false)
	{

	}

	SpinSleeper::~SpinSleeper()
	{

	}

	void SpinSleeper::Spin(nanoseconds time)
	{
		if (time <= 0)
			return;

		m_Timer.Start();
		float totalTime = (float)time;
		float now = m_Timer.GetTimeNano();
		float deltaTime = totalTime - now;
		while (deltaTime > 0.0f && m_Running)
		{
			now = m_Timer.GetTimeNano();
			deltaTime = totalTime - now;
		}
	}

	void SpinSleeper::Sleep(nanoseconds time)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(time));
	}

	void SpinSleeper::SpinSleep(nanoseconds totalTime, nanoseconds sleepTime)
	{
		if (sleepTime > totalTime)
			return;
		auto spinTime = totalTime - sleepTime;
		Sleep(sleepTime);
		Spin(spinTime);
	}

}