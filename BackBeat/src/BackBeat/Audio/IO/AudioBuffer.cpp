#include "bbpch.h"

#include "AudioBuffer.h"
namespace BackBeat {

	AudioBuffer::AudioBuffer(unsigned int bufferSize)
		:
		m_Size(bufferSize),
		m_Buffer(new float[bufferSize])
	{

	}

	AudioBuffer::~AudioBuffer()
	{

	}
}