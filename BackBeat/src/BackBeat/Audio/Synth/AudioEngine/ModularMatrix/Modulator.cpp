#include "bbpch.h"

#include "Modulator.h"
namespace BackBeat {

	Modulator::Modulator(UINT32 bufferSize)
		: m_BufferSize(bufferSize), m_Buffer(std::make_shared<float[]>(bufferSize))
	{

	}
	
	Modulator::~Modulator()
	{

	}

	void Modulator::FlushBuffer(UINT32 numSamples, float defaultValue)
	{
		Audio::FlushBuffer(m_Buffer, numSamples, STEREO, defaultValue);
	}
}