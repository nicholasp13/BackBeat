#pragma once

// Takes all audio processors output busses and mixes them into on renderer
// Performs all sample rate and data conversions

#include "BackBeat/Audio/Audio.h"
#include "AudioProcessor.h"
namespace BackBeat {

	class Mixer
	{
	public:
		Mixer(AudioProps props);
		~Mixer();

		void GetData(byte* data, unsigned int numSamples);
		void SetProc(std::shared_ptr<AudioProcessor> proc);

	private:
		unsigned int m_NumProcs;
		AudioProps m_Props;
		std::shared_ptr <AudioProcessor> m_Proc;
		std::vector < std::shared_ptr<AudioProcessor> > m_Procs;
		
		float GetTypeRatio(unsigned short bitDepth1, unsigned short bitDepth2);
	};
}