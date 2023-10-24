#include "bbpch.h"

#include "AudioBuffer.h"
namespace BackBeat {

	AudioBuffer::AudioBuffer(tWAVEFORMATEX props)
		:
		m_SampleRate(props.nSamplesPerSec),
		m_Size(props.nSamplesPerSec),
		m_Buffer(new float[props.nSamplesPerSec]),
		m_DeviceProps(props)
	{

	}

	AudioBuffer::~AudioBuffer()
	{

	}
}