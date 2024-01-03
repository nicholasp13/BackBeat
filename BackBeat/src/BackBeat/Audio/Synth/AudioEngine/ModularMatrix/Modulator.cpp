#include "bbpch.h"

#include "Modulator.h"
namespace BackBeat {

	Modulator::Modulator(unsigned int bufferSize)
		: m_BufferSize(bufferSize), m_Buffer(std::make_shared<float[]>(bufferSize))
	{

	}
	
	Modulator::~Modulator()
	{

	}

	void Modulator::FlushBuffer(unsigned int numSamples, float defaultValue)
	{
		Audio::FlushBuffer(m_Buffer, numSamples, Audio::Stereo, defaultValue);
	}
}