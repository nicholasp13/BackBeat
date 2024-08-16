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
		m_RecordingSink.Init(m_Props);
		m_Mixer.Init(m_Props, &m_RecordingSink);
		m_Renderer.SetMixer(&m_Mixer);

		m_AudioRecorder.Init(m_Props, &m_RecordingSink);
		m_DeviceRecorder.Init();
		m_RecorderMgr.Init(&m_AudioRecorder, &m_DeviceRecorder);
		m_Mixer.AddRecordingManager(&m_RecorderMgr);

	}

	void WindowsAudioSystem::Start()
	{
		m_Renderer.Start();
	}

	void WindowsAudioSystem::Stop()
	{
		m_Renderer.Stop();
	}

}