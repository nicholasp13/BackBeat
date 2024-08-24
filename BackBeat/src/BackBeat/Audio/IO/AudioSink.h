#pragma once

#include "BackBeat/Audio/Helpers/CircularBuffer.h"
#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class AudioSink
	{
	public:
		AudioSink();
		~AudioSink();

		inline void Init(AudioProps props) { m_Props = props; }
		bool WriteData(float* data, unsigned int numSamples) { return m_Sink.Write(data, numSamples); }
		bool ReadData(float* data, unsigned int numSamples) { return m_Sink.Read(data, numSamples); }
		
		inline void Reset() { m_Sink.Reset(); }

	private:
		AudioProps m_Props;
		CircularBuffer m_Sink;

	};

}