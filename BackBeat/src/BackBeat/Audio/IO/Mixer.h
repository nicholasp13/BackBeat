#pragma once

// Takes all audio processors output busses and mixes them into on renderer
// Performs data conversions

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Recorder/RecorderManager.h"
#include "AudioProcessor.h"
namespace BackBeat {

	class Mixer
	{
	public:
		Mixer(AudioProps props);
		~Mixer();

		void GetData(byte* data, unsigned int numSamples);
		void PushProcessor(std::shared_ptr<AudioProcessor> processor);

		inline void AddRecordingManager(std::shared_ptr<RecorderManager> recordingManager) { m_RecordingManager = recordingManager; }

	private:
		unsigned int m_NumProcessors;
		AudioProps m_Props;
		UUID m_ID;
		std::vector < std::shared_ptr<AudioProcessor> > m_Processors;
		
		std::shared_ptr<RecorderManager> m_RecordingManager;
	};
}