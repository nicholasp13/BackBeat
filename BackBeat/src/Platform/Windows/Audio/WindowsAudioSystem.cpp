#include "bbpch.h"

#include "BackBeat/File/FileSystem.h"
#include "WindowsAudioSystem.h"
namespace BackBeat {

	WindowsAudioSystem::WindowsAudioSystem()
		: m_Props(AudioProps())
	{

	}

	WindowsAudioSystem::~WindowsAudioSystem()
	{
		m_RecorderMgr.Stop();
		m_PlayerMgr.StopAll();
		m_Renderer.Stop();
		m_Thread.Stop();
	}

	void WindowsAudioSystem::Init(std::string appName)
	{
		// Creates filepath to directory to store 
		std::string appDataLocalDir = BackBeat::Windows::GetLocalAppFileDirectory();
		BackBeat::FileSystem::SetAppDataLocalDir(appDataLocalDir);
		BackBeat::FileSystem::SetAppDir(appName);
		BackBeat::FileSystem::CreateTempDir();
		BackBeat::FileSystem::ClearDir(BackBeat::FileSystem::GetTempDir());

		BackBeat::TrackFactory::SetFileMapper(&m_FileMapper);

		m_Renderer.Init();

		m_Props = m_Renderer.GetProps();
		m_Props.format = Audio::GetAudioFormat(m_Props.bitDepth, false);

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