#include "bbpch.h"

#include "BackBeat/Core/FileSystem.h"
#include "WindowsAudioSystem.h"
namespace BackBeat {

	WindowsAudioSystem::WindowsAudioSystem()
		: m_Props(AudioProps())
	{

	}

	WindowsAudioSystem::~WindowsAudioSystem()
	{
		m_Renderer.Stop();
		m_Thread.Stop();
	}

	void WindowsAudioSystem::Init(std::string projectName)
	{
		// Creates filepath to directory to store 
		std::string appDataLocalDir = BackBeat::Windows::GetLocalAppFileDirectory();
		BackBeat::FileSystem::SetAppDataLocalDir(appDataLocalDir);
		BackBeat::FileSystem::SetProjectDir(projectName);
		BackBeat::FileSystem::CreateTempDir();

		m_Renderer.Init();
		m_Props = m_Renderer.GetProps();
		m_Engine.Init(m_Props, &m_Renderer, &m_RecorderMgr, true);

		m_AudioRecorder.Init(m_Props, m_Engine.GetRecordingSink());
		m_DeviceRecorder.Init();
		m_RecorderMgr.Init(&m_AudioRecorder, &m_DeviceRecorder);
	}

	void WindowsAudioSystem::Start()
	{
		std::function<void()> callback = std::bind(&AudioEngine::Callback, &m_Engine);
		m_Thread.Start(callback, 0, true, false);
	}

	void WindowsAudioSystem::Stop()
	{
		m_Renderer.Stop();
		m_Thread.Stop(); 
	}

}