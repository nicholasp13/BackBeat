#include "bbpch.h"

#include "AudioSink.h"
namespace BackBeat {

	AudioSink::AudioSink()
		: m_Props(AudioProps(0))
	{

	}

	AudioSink::~AudioSink()
	{

	}

	// TODO: Make inline if not expanded
	void AudioSink::Init(AudioProps props)
	{
		m_Props = props;
	}

	// TODO: Make inline if not expanded
	bool AudioSink::WriteData(float* data, unsigned int numSamples)
	{
		return m_Sink.Write(data, numSamples);
	}

	// TODO: Make inline if not expanded
	bool AudioSink::ReadData(float* data, unsigned int numSamples)
	{
		return m_Sink.Read(data, numSamples);
	}

}