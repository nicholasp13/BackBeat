#pragma once

// Takes all audio processors output busses and mixes them into on renderer
// Performs data conversions

#include "BackBeat/Audio/Audio.h"
#include "AudioProcessor.h"
namespace BackBeat {

	class Mixer
	{
	public:
		Mixer(AudioProps props);
		~Mixer();

		void GetData(byte* data, unsigned int numSamples);
		void PushProcessor(std::shared_ptr<AudioProcessor> processor);

	private:
		unsigned int m_NumProcessors;
		AudioProps m_Props;
		std::vector < std::shared_ptr<AudioProcessor> > m_Processors;
		
	};
}