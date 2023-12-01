#include "bbpch.h"

#include "AudioBus.h"
namespace BackBeat {

	AudioBus::AudioBus(AudioProps props, BusDirection direction)
		: m_Direction(direction), m_Props(props), m_Buffer(AudioBuffer(props.sampleRate))
	{

	}

	AudioBus::~AudioBus()
	{

	}
}