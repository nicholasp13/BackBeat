#pragma once

#include "BackBeat/Audio/Helpers/CircularBuffer.h"
#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class AudioSink
	{
	public:
		AudioSink();
		~AudioSink();

		void Init(AudioProps props);
		bool WriteData(float* data, unsigned int numSamples);
		bool ReadData(float* data, unsigned int numSamples);
		
		inline void Reset() { m_Sink.Reset(); }

	private:
		AudioProps m_Props;
		CircularBuffer m_Sink;

	};

}