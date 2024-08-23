#pragma once

#include "BackBeat/Audio/IO/Mixer.h"
#include "BackBeat/Audio/Renderer/Renderer.h"
#include "Audio.h"
namespace BackBeat {

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void Init(AudioProps props, Renderer* renderer, RecorderManager* recordingMgr, bool free);
		
		void Callback();

		inline AudioProps GetProps() { return m_Props; }
		inline Mixer* GetMixer() { return &m_Mixer; }
		inline Renderer* GetRenderer() { return m_Renderer; }
		inline AudioSink* GetRecordingSink() { return &m_RecordingSink; }

	private:
		bool m_Init;
		bool m_Free;
		AudioProps m_Props;
		AudioSink m_RecordingSink;
		Mixer m_Mixer;
		Renderer* m_Renderer;

	};

}