#pragma once

#include "WindowsMIDIDeviceManager.h"
#include "WindowsRecorder.h"
#include "WindowsRenderer.h"
#include "BackBeat/Audio/Recorder/AudioRecorder.h"
#include "BackBeat/Audio/AudioSystem.h"
namespace BackBeat {

	class WindowsAudioSystem : public AudioSystem
	{
	public:
		WindowsAudioSystem();
		~WindowsAudioSystem();

		virtual void Init(std::string projectName) override;
		virtual void Start() override;
		virtual void Stop() override;

		inline virtual AudioProps GetProps() override { return m_Props; }
		inline virtual Mixer* GetMixer() override { return m_Engine.GetMixer(); }
		inline virtual Renderer* GetRenderer() override { return &m_Renderer; }
		inline virtual PlayerManager* GetPlayerManager() override { return &m_PlayerMgr; }
		inline virtual RecorderManager* GetRecorderManager() override { return &m_RecorderMgr; }
		inline virtual MIDIDeviceManager* GetMIDIDeviceManager() override { return &m_MIDIDeviceMgr; }
		inline virtual Visualizer* GetVisualizer() override { return m_Engine.GetVisualizer(); }

	private:
		AudioProps m_Props;
		AudioEngine m_Engine;
		AudioRecorder m_AudioRecorder;
		PlayerManager m_PlayerMgr;
		RecorderManager m_RecorderMgr;
		AudioThread m_Thread;

		WindowsRecorder m_DeviceRecorder;
		WindowsRenderer m_Renderer;
		WindowsMIDIDeviceManager m_MIDIDeviceMgr;
	};

}