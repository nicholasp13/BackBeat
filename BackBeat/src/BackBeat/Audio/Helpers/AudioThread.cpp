#include "bbpch.h"

#include "AudioThread.h"
namespace BackBeat {

	AudioThread::AudioThread()
		: 
		m_Running(false), 
		m_Free(false),
		m_Spin(false),
		m_SleepTime(0ll)
	{

	}

	AudioThread::~AudioThread()
	{
		Stop();
	}
	
	// @params
	//     free - This determines whether AudioThread loops itself or the callback is free to do whatever after it is called
	void AudioThread::Start(std::function<void()> callback, long long sleepTime, bool free, bool spin)
	{
		m_CallBack = callback;
		m_SleepTime = sleepTime;
		m_Free = free;
		m_Spin = spin;

		if (m_Free)
			m_Thread = std::thread(&AudioThread::RunFree, this);
		else
			m_Thread = std::thread(&AudioThread::Run, this);
	}

	void AudioThread::Stop()
	{
		m_Running = false;
		m_SleepTime = 0;
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void AudioThread::Run()
	{
		SpinSleeper spinner;

		while (m_Running)
		{
			if (m_Spin)
				spinner.Spin(m_SleepTime);
			else
				spinner.Sleep(m_SleepTime);

			m_CallBack();
		}
	}

	void AudioThread::RunFree()
	{
		m_CallBack();
		m_Running = false;
	}
}