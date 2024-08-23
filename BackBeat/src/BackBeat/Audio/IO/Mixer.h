#pragma once

// Takes all audio processors output busses and mixes them into on renderer
// Performs data conversions

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/IO/AudioSink.h"
#include "BackBeat/Audio/Recorder/RecorderManager.h"
#include "AudioProcessor.h"
namespace BackBeat {

	class Mixer
	{
	public:
		Mixer();
		~Mixer();

		void Init(AudioProps props, AudioSink* sink);
		void RenderData(byte* data, unsigned int numFrames);
		void RenderData(unsigned int numFrames);
		void DeleteProcessor(UUID id);

		inline void Reset() { m_Sink->Reset(); }
		inline void PushProcessor(std::shared_ptr<AudioProcessor> processor) { m_Processors.push_back(processor); }
		inline void SetRecordingManager(RecorderManager* recordingManager) { m_RecordingManager = recordingManager; }

	private:
		static const unsigned int s_BufferSize = 50000;

		std::shared_ptr<byte[]> m_Buffer;
		UUID m_ID;
		AudioProps m_Props;
		AudioSink* m_Sink;
		std::vector < std::shared_ptr<AudioProcessor> > m_Processors;
		RecorderManager* m_RecordingManager;

	};
}