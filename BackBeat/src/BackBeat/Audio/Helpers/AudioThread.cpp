#include "bbpch.h"

#include "AudioThread.h"
namespace BackBeat {

	AudioThread::AudioThread()
		: m_Running(false), m_SleepTime(0ll)
	{

	}

	AudioThread::~AudioThread()
	{
		Stop();
	}
	
	// @params
	//     free - This determines whether AudioThread loops itself or the callback is free to do whatever after it is called
	void AudioThread::Start(std::function<void()> callback, long long sleepTime, bool free)
	{
		m_Running = true;
		m_SleepTime = sleepTime;
		m_CallBack = callback;
		if (free)
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
			// Should find way to specify whether the thread should sleep or spin depending on importance
			spinner.Spin(m_SleepTime);
			m_CallBack();
		}
	}

	void AudioThread::RunFree()
	{
		m_CallBack();
	}
}