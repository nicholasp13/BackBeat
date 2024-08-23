#pragma once

#include <thread>
#include <functional>

#include "SpinSleeper.h"
namespace BackBeat {

	class AudioThread
	{
	public:
		AudioThread();
		~AudioThread();

		void Start(std::function<void()> callback, long long sleepTime, bool free, bool spin);
		void Stop();

	private:
		bool m_Running;
		bool m_Free;
		bool m_Spin;
		long long m_SleepTime;
		std::thread m_Thread;
		std::function<void()> m_CallBack;

	private:
		void Run();
		void RunFree();
	};

}